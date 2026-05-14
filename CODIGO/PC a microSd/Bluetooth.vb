Imports Windows.Devices.Bluetooth
Imports Windows.Devices.Bluetooth.Advertisement
Imports Windows.Devices.Bluetooth.GenericAttributeProfile
Imports Windows.Devices.Radios
Imports Windows.Security.Cryptography
Imports Windows.Storage.Streams

Public Class Bluetooth

    Private Shared ReadOnly UUID_SERVICIO As New Guid("0000FFE0-0000-1000-8000-00805F9B34FB")
    Private Shared ReadOnly UUID_CARACTERISTICA As New Guid("0000FFE1-0000-1000-8000-00805F9B34FB")

    Private _watcher As BluetoothLEAdvertisementWatcher
    Private _dispositivos As New Dictionary(Of ULong, String)

    Private Shared _device As BluetoothLEDevice
    Private Shared _servicio As GattDeviceService
    Private Shared _caracteristica As GattCharacteristic

    Public Delegate Sub DispositivoEncontradoHandler(nombre As String, direccion As ULong)
    Public Delegate Sub DispositivoEliminadoHandler(id As String)
    Public Delegate Sub BusquedaCompletadaHandler()
    Public Delegate Sub DispositivoActualizadoHandler(nombre As String, direccion As ULong)

    Public Event DispositivoEncontrado As DispositivoEncontradoHandler
    Public Event DispositivoEliminado As DispositivoEliminadoHandler
    Public Event BusquedaCompletada As BusquedaCompletadaHandler
    Public Event DispositivoActualizado As DispositivoActualizadoHandler

    Public Sub IniciarBusqueda()
        If _watcher IsNot Nothing Then DetenerBusqueda()

        _watcher = New BluetoothLEAdvertisementWatcher()
        _watcher.ScanningMode = BluetoothLEScanningMode.Active

        AddHandler _watcher.Received,
            Sub(sender As BluetoothLEAdvertisementWatcher,
                args As BluetoothLEAdvertisementReceivedEventArgs)

                Dim direccion = args.BluetoothAddress
                Dim nombre = args.Advertisement.LocalName

                SyncLock _dispositivos
                    If Not _dispositivos.ContainsKey(direccion) Then
                        ' Primera vez: añadir aunque no tenga nombre
                        _dispositivos(direccion) = If(String.IsNullOrWhiteSpace(nombre),
                                              $"(sin nombre) [{DireccionMac(direccion)}]",
                                              nombre)
                        RaiseEvent DispositivoEncontrado(_dispositivos(direccion), direccion)

                    ElseIf Not String.IsNullOrWhiteSpace(nombre) AndAlso
                   _dispositivos(direccion).StartsWith("(sin nombre)") Then
                        ' Llegó el nombre en un paquete posterior → actualizar
                        _dispositivos(direccion) = nombre
                        RaiseEvent DispositivoActualizado(nombre, direccion)
                    End If
                End SyncLock
            End Sub

        AddHandler _watcher.Stopped,
            Sub(sender As BluetoothLEAdvertisementWatcher,
                args As BluetoothLEAdvertisementWatcherStoppedEventArgs)
                RaiseEvent BusquedaCompletada()
            End Sub

        _dispositivos.Clear()
        _watcher.Start()
    End Sub

    Public Sub DetenerBusqueda()
        If _watcher IsNot Nothing AndAlso
           _watcher.Status = BluetoothLEAdvertisementWatcherStatus.Started Then
            _watcher.Stop()
        End If
    End Sub

    Public Shared Function DireccionMac(direccion As ULong) As String
        Return String.Format("{0:X2}:{1:X2}:{2:X2}:{3:X2}:{4:X2}:{5:X2}",
            (direccion >> 40) And &HFF,
            (direccion >> 32) And &HFF,
            (direccion >> 24) And &HFF,
            (direccion >> 16) And &HFF,
            (direccion >> 8) And &HFF,
            direccion And &HFF)
    End Function

    Public Shared Async Function EscanearYConectarAsync(direccion As ULong) As Task(Of GattCharacteristic)

        Dim watcher = New BluetoothLEAdvertisementWatcher()
        watcher.ScanningMode = BluetoothLEScanningMode.Active

        Dim encontrado As New TaskCompletionSource(Of Boolean)()

        AddHandler watcher.Received, Sub(w, args)
                                         If args.BluetoothAddress = direccion Then
                                             encontrado.TrySetResult(True)
                                         End If
                                     End Sub

        watcher.Start()
        Dim visto = Await Task.WhenAny(encontrado.Task, Task.Delay(8000))
        watcher.Stop()

        If visto IsNot encontrado.Task Then
            Throw New Exception("Dispositivo no encontrado en escaneo")
        End If

        Debug.WriteLine("[BT] Dispositivo visto, obteniendo MAC adaptador...")

        ' Obtener MAC del adaptador local
        Dim adaptadorSelector = BluetoothAdapter.GetDeviceSelector()
        Dim adaptadores = Await Windows.Devices.Enumeration.DeviceInformation.FindAllAsync(adaptadorSelector)
        If adaptadores.Count = 0 Then Throw New Exception("Adaptador BT no encontrado")

        Dim adapter = Await BluetoothAdapter.FromIdAsync(adaptadores(0).Id)
        Dim macLocal = DireccionMac(adapter.BluetoothAddress)
        Dim macRemota = DireccionMac(direccion)

        ' Construir DeviceId
        Dim deviceId = $"BluetoothLE#BluetoothLE{macLocal}-{macRemota}"
        Debug.WriteLine("[BT] DeviceId: " & deviceId)

        _device = Await BluetoothLEDevice.FromIdAsync(deviceId)
        Debug.WriteLine("[BT] _device = " & (_device IsNot Nothing).ToString())

        If _device Is Nothing Then Throw New Exception("No se pudo conectar")

        Dim serviciosResult = Await _device.GetGattServicesForUuidAsync(
        UUID_SERVICIO, BluetoothCacheMode.Cached)

        Debug.WriteLine("[BT] Servicios: " & serviciosResult.Status.ToString())

        If serviciosResult.Status <> GattCommunicationStatus.Success OrElse
       serviciosResult.Services.Count = 0 Then
            Throw New Exception("Servicio FFE0 no encontrado")
        End If

        _servicio = serviciosResult.Services(0)

        Dim caracteristicasResult = Await _servicio.GetCharacteristicsForUuidAsync(
        UUID_CARACTERISTICA, BluetoothCacheMode.Cached)

        Debug.WriteLine("[BT] Caracteristicas: " & caracteristicasResult.Status.ToString())

        If caracteristicasResult.Status <> GattCommunicationStatus.Success OrElse
       caracteristicasResult.Characteristics.Count = 0 Then
            Throw New Exception("Característica FFE1 no encontrada")
        End If

        _caracteristica = caracteristicasResult.Characteristics(0)
        Return _caracteristica
    End Function

    Public Shared Async Function EnviarAsync(caracteristica As GattCharacteristic, texto As String) As Task

        ' enviar en mensaje en partes de 20 bytes
        Dim bytes() As Byte = System.Text.Encoding.UTF8.GetBytes(texto)
        Dim chunkSize As Integer = 20

        Dim i As Integer = 0
        While i < bytes.Length
            Dim length As Integer = Math.Min(chunkSize, bytes.Length - i)
            Dim chunk(length - 1) As Byte
            Array.Copy(bytes, i, chunk, 0, length)

            Dim buffer = CryptographicBuffer.CreateFromByteArray(chunk)
            Await caracteristica.WriteValueAsync(buffer, GattWriteOption.WriteWithoutResponse)
            Await Task.Delay(30)
            i += chunkSize
        End While
    End Function

    Public Shared Async Function SuscribirNotificacionesAsync(
        caracteristica As GattCharacteristic,
        callback As Action(Of String)) As Task

        Await caracteristica.WriteClientCharacteristicConfigurationDescriptorAsync(
            GattClientCharacteristicConfigurationDescriptorValue.Notify)

        AddHandler caracteristica.ValueChanged,
            Sub(sender As GattCharacteristic, args As GattValueChangedEventArgs)
                Dim texto = CryptographicBuffer.ConvertBinaryToString(
    BinaryStringEncoding.Utf8,
    args.CharacteristicValue)
                callback(texto)
            End Sub
    End Function

    Public Function ObtenerTodos() As Dictionary(Of ULong, String)
        SyncLock _dispositivos
            Return New Dictionary(Of ULong, String)(_dispositivos)
        End SyncLock
    End Function

    Public Shared Function NombreDispositivo(nombre As String, direccion As ULong) As String
        If Not String.IsNullOrWhiteSpace(nombre) Then Return nombre
        Return $"(sin nombre) [{DireccionMac(direccion)}]"
    End Function

    Public Shared Async Function DesconectarAsync() As Task
        Try
            If _caracteristica IsNot Nothing Then
                Await _caracteristica.WriteClientCharacteristicConfigurationDescriptorAsync(
                    GattClientCharacteristicConfigurationDescriptorValue.None)
            End If
        Catch
            ' Ignorar errores al desuscribir — puede que ya esté desconectado
        End Try

        _caracteristica = Nothing
        _servicio?.Dispose()
        _servicio = Nothing
        _device?.Dispose()
        _device = Nothing
    End Function

    'versión síncrona para llamar desde el botón de cerrar el formulario, etc...
    Public Shared Sub DesconectarSync()
        _caracteristica = Nothing
        _servicio?.Dispose()
        _servicio = Nothing
        _device?.Dispose()
        _device = Nothing
    End Sub

    Private Shared Async Function ObtenerMacAdaptadorAsync() As Task(Of String)
        Dim radios = Await Radio.GetRadiosAsync()
        Dim bt = radios.FirstOrDefault(Function(r) r.Kind = RadioKind.Bluetooth)
        If bt Is Nothing Then Throw New Exception("No hay adaptador Bluetooth")

        ' La MAC del adaptador está en el nombre del radio en Windows
        Dim selector = BluetoothAdapter.GetDeviceSelector()
        Dim devices = Await Windows.Devices.Enumeration.DeviceInformation.FindAllAsync(selector)
        If devices.Count = 0 Then Throw New Exception("Adaptador no encontrado")

        Dim adapter = Await BluetoothAdapter.FromIdAsync(devices(0).Id)
        Return DireccionMac(adapter.BluetoothAddress)
    End Function

End Class