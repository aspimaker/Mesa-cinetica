' ==================================================
' Mesa Cinética - Kinetic Table
' Copyright © 2026 aspimaker
'
'https://github.com/aspimaker/Mesa-cinetica/
'
' Este software está bajo licencia Creative Commons
' Attribution-NonCommercial-ShareAlike 4.0 International
' CC BY-NC-SA 4.0
' 
' Usted es libre de:
'   • Compartir: copiar y redistribuir el material
'   • Adaptar: remezclar, transformar y construir sobre el material
' 
' exclusivamente bajo los siguientes términos:
'   • Atribución: debe dar crédito adecuado
'   • No Comercial: no puede usar el material con fines comerciales
'   • Compartir Igual: si remezcla o transforma, debe distribuir bajo la misma licencia
' ==================================================

Imports System.ComponentModel
Imports System.ComponentModel.DataAnnotations
Imports System.IO
Imports System.Net.NetworkInformation
Imports System.Runtime.InteropServices
Imports System.Security.Cryptography
Imports System.Text
Imports System.Threading
Imports Microsoft.Data.Sqlite
Imports SkiaSharp
Imports SkiaSharp.Views.Desktop
Imports TagLib
Imports Windows.Devices.Bluetooth.GenericAttributeProfile
Imports Windows.Devices.Radios

Public Class Form1

    Private _finder As New Bluetooth
    Private _mesas As New List(Of String)

    Private idMesa As String = "aspiKntc"
    Private _caracteristica As GattCharacteristic
    Private _procesandoDragDrop As Boolean = False

    Private _colorRGB As Color = Color.White

    Private totalArchivos As Integer = 0

    Private _play As Boolean = False
    Private _pausa As Boolean = False
    Private _anterior As Boolean = False
    Private _siguiente As Boolean = False
    Private _stop As Boolean = False

    Private _repeticionNormal As Boolean = False



    Private _pista As Integer = 0
    Private _totalPistas As Integer = 0
    Private _offsetPista As Integer = 10 'nº de ficheros con sonidos de aviso del sistema

    ' Constantes de WM_DEVICECHANGE
    Private Const WM_DEVICECHANGE As Integer = &H219
    Private Const DBT_DEVICEARRIVAL As Integer = &H8000       ' Unidad conectada
    Private Const DBT_DEVICEREMOVECOMPLETE As Integer = &H8004 ' Unidad desconectada
    Private Const DBT_DEVTYP_VOLUME As Integer = &H2

    ' Estructura para identificar el volumen
    <StructLayout(LayoutKind.Sequential)>
    Private Structure DEV_BROADCAST_VOLUME
        Dim dbcv_size As Integer
        Dim dbcv_devicetype As Integer
        Dim dbcv_reserved As Integer
        Dim dbcv_unitmask As Integer
        Dim dbcv_flags As Short
    End Structure


    Private Sub Form1_Load(sender As Object, e As EventArgs) Handles MyBase.Load

        AddHandler _finder.DispositivoActualizado, New Bluetooth.DispositivoActualizadoHandler(AddressOf OnDispositivoActualizado)
        AddHandler _finder.DispositivoEncontrado, New Bluetooth.DispositivoEncontradoHandler(AddressOf OnDispositivoEncontrado)
        AddHandler _finder.DispositivoEliminado, New Bluetooth.DispositivoEliminadoHandler(AddressOf OnDispositivoEliminado)
        AddHandler _finder.BusquedaCompletada, New Bluetooth.BusquedaCompletadaHandler(AddressOf OnBusquedaCompletada)

        ' Dim radio = Await Radio.GetRadiosAsync()
        'Dim bt = radio.FirstOrDefault(Function(r) r.Kind = RadioKind.Bluetooth)
        'If bt Is Nothing OrElse bt.State <> RadioState.On Then
        'Throw New Exception("Bluetooth no disponible")
        'End If

        LblEspacio.Text = "Conectar microSD"

        imgCaratula.SizeMode = PictureBoxSizeMode.Zoom

        ' Asignar eventos (solo necesario si no los asignaste en el diseñador)
        AddHandler listaMP3.DragEnter, AddressOf listaMP3_DragEnter
        AddHandler listaMP3.DragDrop, AddressOf listaMP3_DragDrop

        conexion = New SqliteConnection()

        ' iniciar la búsqueda
        _finder.IniciarBusqueda()

        cbPatron.Items.AddRange({"Áster", "Mariposa", "Cardioide", "Trocoide", "Espiral doble", "Moiré", "Curva de Hilbert", "Patrón áureo (Fibonacci)", "Girasol (Fibonacci)", "Estrella", "Círculos concéntricos", "Espiral de Fermat", "Espiral logarítmica", "Epicicloide", "Hipocicloide", "Espiral", "Rosa", "Lissajous"})
        cbPatron.SelectedIndex = 0

        cbModoRGB.Items.AddRange({"Apagado", "Sólido", "Arcoíris", "Pulso", "Fuego", "Persecución"})
        cbModoRGB.SelectedIndex = 2 ' Arcoíris por defecto

        cbModoRGB_SelectedIndexChanged(Nothing, Nothing)
        'ColorPickerControl1.BackColor = _colorRGB
        ColorPickerControl1.BackColor = Color.Transparent

        tbBrilloRGB.Minimum = 0
        tbBrilloRGB.Maximum = 100

        tbRotacion.Minimum = 0
        tbRotacion.Maximum = 359
        tbRotacion.Value = 0

        tbVolumen.Minimum = 0
        tbVolumen.Maximum = 30
        tbVolumen.Value = 0

        SkControl1.Invalidate() 'forzar redibujado

        gbControlMesa.Enabled = False


        'botones del reproductor
        For Each ctrl As Control In PanelBotones.Controls
            If TypeOf ctrl Is Button Then
                Dim b As Button = DirectCast(ctrl, Button)

                b.FlatStyle = FlatStyle.Flat
                b.BackColor = Color.Silver
                b.ForeColor = Color.Black
                b.FlatAppearance.BorderColor = Color.DimGray
                'b.FlatAppearance.BorderSize = 2
                b.Font = New Font("Segoe UI", 16, FontStyle.Bold)
                'b.Top = 10
            End If
        Next

        btAnterior.Text = "◀◀"
        btPlay.Text = "▶"
        btPausa.Text = "❚❚"
        btSiguiente.Text = "▶▶"
        btStop.Text = "⏏"


        'botones modo reproducción
        For Each ctrl As Control In panelModoRepeticion.Controls
            If TypeOf ctrl Is Button Then
                Dim b As Button = DirectCast(ctrl, Button)
                b.FlatStyle = FlatStyle.Flat
                b.BackColor = Color.Silver
                b.ForeColor = Color.Black
                b.FlatAppearance.BorderColor = Color.DimGray
                'b.FlatAppearance.BorderSize = 2
                b.Font = New Font("Segoe UI", 16, FontStyle.Bold)
                'b.Top = 10
            End If
        Next

        btNormal.Activado = True

        btNormal.Icono = "🔁"
        btNormal.TextoBoton = "Normal"

        btRepetirTodas.Icono = "🔂"
        btRepetirTodas.TextoBoton = "Repetir todas"

        btRepetirCancion.Icono = "🔄"
        btRepetirCancion.TextoBoton = "Repetir una"

        btCancionAleatoria.Icono = "🔀"
        btCancionAleatoria.TextoBoton = "Aleatorio"

        AplicarColoresBotonesPanel(SystemColors.Control, Color.Green, SystemColors.ControlText, panelModoRepeticion)



        'botones equalización
        For Each ctrl As Control In panelEqualizacion.Controls
            If TypeOf ctrl Is Button Then
                Dim b As Button = DirectCast(ctrl, Button)
                b.FlatStyle = FlatStyle.Flat
                b.BackColor = Color.Silver
                b.ForeColor = Color.Black
                b.FlatAppearance.BorderColor = Color.DimGray
                'b.FlatAppearance.BorderSize = 2
                b.Font = New Font("Segoe UI", 16, FontStyle.Bold)
                'b.Top = 10
            End If
        Next

        btEqNormal.Activado = True

        btEqNormal.Icono = "🎵"
        btEqNormal.TextoBoton = "Normal"

        btEqPop.Icono = "🎤"
        btEqPop.TextoBoton = "Pop"

        btEqRock.Icono = "🤘"
        btEqRock.TextoBoton = "Rock"

        btEqJazz.Icono = "🎷"
        btEqJazz.TextoBoton = "Jazz"

        btEqClassic.Icono = "🎻"
        btEqClassic.TextoBoton = "Classic"

        btEqBass.Icono = "🔊"
        btEqBass.TextoBoton = "Bass"

        AplicarColoresBotonesPanel(SystemColors.Control, Color.Orange, SystemColors.ControlText, panelEqualizacion)


    End Sub

    Private Sub Form1_FormClosing(sender As Object, e As FormClosingEventArgs) Handles MyBase.FormClosing
        _finder.DetenerBusqueda()
    End Sub

    Private Sub BtnBuscar_Click(sender As Object, e As EventArgs) Handles BtnBuscar.Click
        ListBoxDispositivos.Items.Clear()
        LabelEstado.Text = "Buscando..."
        BtnBuscar.Enabled = False
        BtnDetener.Enabled = True
        _finder.IniciarBusqueda()
    End Sub

    Private Sub BtnDetener_Click(sender As Object, e As EventArgs) Handles BtnDetener.Click
        _finder.DetenerBusqueda()
        BtnBuscar.Enabled = True
        BtnDetener.Enabled = False
    End Sub

    Private Sub OnDispositivoEncontrado(nombre As String, direccion As ULong)
        If InvokeRequired Then
            Invoke(Sub() OnDispositivoEncontrado(nombre, direccion))
            Return
        End If
        ListBoxDispositivos.Items.Add($"{nombre}  —  {Bluetooth.DireccionMac(direccion)}")
    End Sub

    Private Sub OnDispositivoEliminado(id As String)
        If InvokeRequired Then
            Invoke(Sub() OnDispositivoEliminado(id))
            Return
        End If
        ' Quitar el item cuyo texto contiene el id
        Dim item = ListBoxDispositivos.Items.Cast(Of String).FirstOrDefault(Function(s) s.Contains(id))
        If item IsNot Nothing Then ListBoxDispositivos.Items.Remove(item)
    End Sub

    Private Sub OnBusquedaCompletada()
        'If InvokeRequired Then
        ' Invoke(Sub() OnBusquedaCompletada())
        'Return
        'End If

        'LabelEstado.Text = $"Búsqueda completada — {ListBoxDispositivos.Items.Count} dispositivo(s) encontrado(s)."
        'BtnBuscar.Enabled = True
        'BtnDetener.Enabled = False
    End Sub

    Private Sub OnDispositivoActualizado(nombre As String, direccion As ULong)

        If InvokeRequired Then
            Invoke(Sub() OnDispositivoActualizado(nombre, direccion))
            Return
        End If

        ' Buscar esa dirección y actualizar el nombre
        Dim mac = Bluetooth.DireccionMac(direccion)
        For i = 0 To ListBoxDispositivos.Items.Count - 1
            If ListBoxDispositivos.Items(i).ToString().Contains(mac) Then
                ListBoxDispositivos.Items(i) = $"{nombre}  —  {mac}"

                If nombre.StartsWith(idMesa) Then
                    ListaMesas.Items.Add($"{nombre}|{direccion}")
                    _mesas.Add($"{nombre}|{direccion}")
                End If

                If ListaMesas.Items.Count > 0 Then
                    If ListaMesas.SelectedItems.Count = 0 Then
                        ListaMesas.SelectedIndex = 0
                    End If
                End If

                Exit For
            End If
        Next
    End Sub


    Private Async Sub BtnEnviar_Click(sender As Object, e As EventArgs) Handles BtnEnviar.Click
        If _caracteristica Is Nothing Then Return
        Try

            Dim _enviar As String = String.Concat(inicioMensaje, TxtEnviar.Text, finMensaje)

            Await Bluetooth.EnviarAsync(_caracteristica, _enviar)
        Catch ex As Exception
            MessageBox.Show($"Error al enviar: {ex.Message}")
        End Try
    End Sub

    Private Async Sub BtnConectar_Click(sender As Object, e As EventArgs) Handles BtnConectar.Click

        If _caracteristica = Nothing Then

            If ListaMesas.SelectedIndex < 0 Then Return

            ' _finder.DetenerBusqueda()

            Try
                LabelEstado.Text = "Conectando..."
                Dim registro = _mesas(ListaMesas.SelectedIndex)
                Dim direccion = registro.Substring(registro.IndexOf("|"c) + 1)

                _caracteristica = Await Bluetooth.EscanearYConectarAsync(direccion)

                Await Bluetooth.SuscribirNotificacionesAsync(_caracteristica,
                    Sub(respuesta)

                        'If InvokeRequired Then
                        'Invoke(Sub() TxtRecibido.Text = respuesta) 'si la llamada viene de otro hilo
                        'Else
                        'TxtRecibido.Text = respuesta ' cuando la llamada es del mismo hilo
                        'End If

                        BeginInvoke(Sub() 'sirve para los dos casos
                                        procesarRespuestaBT(respuesta)
                                    End Sub)
                    End Sub)

                gbControlMesa.Enabled = True
                'gbMP3.Enabled = True
                gbListaPatrones.Enabled = True
                gbPatrones.Enabled = True
                gbRGB.Enabled = True


                LabelEstado.Text = "Conectado"
                BtnConectar.Text = "Desconectar"


                ' con await espera que terminen las llamadas asíncronas
                Await enviarBT(String.Concat("SINCROFECHA", FormatearFechaHora(Now)))
                'Await Task.Delay(200)
                Await enviarBT("GETVOLUMEN") 'solicitar el volumen

                Await enviarBT("GETEQ") 'solicitar la ecualización activada


            Catch ex As Exception
                LabelEstado.Text = "Error al conectar"
                MessageBox.Show($"Error: {ex.Message}", "Error",
                                MessageBoxButtons.OK, MessageBoxIcon.Error)
            End Try

        Else

            gbControlMesa.Enabled = False
            'gbMP3.Enabled = False
            gbListaPatrones.Enabled = False
            gbPatrones.Enabled = False
            gbRGB.Enabled = False


            _caracteristica = Nothing

            LabelEstado.Text = "Desconectado"
            BtnConectar.Text = "Conectar"

            lbCancionesMesa.Items.Clear()

            Await Bluetooth.DesconectarAsync

        End If




    End Sub

    Async Function enviarBT(mensaje As String) As Task

        If _caracteristica Is Nothing Then Return

        Try
            Dim _enviar As String = String.Concat(inicioMensaje, mensaje, finMensaje)

            Await Bluetooth.EnviarAsync(_caracteristica, _enviar)
        Catch ex As Exception

            Debug.Print($"Error al enviar: {ex.Message}")

            _finder.DetenerBusqueda()

            BtnConectar.PerformClick()
            ListBoxDispositivos.Items.Clear()
            ListaMesas.Items.Clear()

            _finder.IniciarBusqueda()

            MsgBox("Se ha perdido la conexión con la mesa.", MsgBoxStyle.Exclamation, "Atención...")

        End Try

    End Function

    Public Function FormatearFechaHora(fecha As DateTime) As String

        Dim diaNumero As Integer = If(fecha.DayOfWeek = DayOfWeek.Sunday, 7, fecha.DayOfWeek)
        Return fecha.ToString("ddMMyyHHmmss") & diaNumero.ToString()

    End Function

    Sub procesarRespuestaBT(respuesta As String)

        Debug.Print(respuesta)
        TxtRecibido.Text = respuesta

        Dim _t As String = respuesta

        Select Case True

            Case respuesta.StartsWith("OK:GETVOLUMEN_")
                tbVolumen.Value = Strings.Right(respuesta, Len(respuesta) - 14)

            Case respuesta.StartsWith("OK:GETEQ_")
                Select Case Strings.Right(respuesta, Len(respuesta) - 9)
                    Case "0"
                        BotonesEQ_Click(btEqNormal, EventArgs.Empty)
                    Case "1"
                        BotonesEQ_Click(btEqPop, EventArgs.Empty)
                    Case "2"
                        BotonesEQ_Click(btEqRock, EventArgs.Empty)
                    Case "3"
                        BotonesEQ_Click(btEqJazz, EventArgs.Empty)
                    Case "4"
                        BotonesEQ_Click(btEqClassic, EventArgs.Empty)
                    Case "5"
                        BotonesEQ_Click(btEqBass, EventArgs.Empty)
                End Select

            Case respuesta.StartsWith("OK:PR_")
                Select Case Strings.Right(respuesta, Len(respuesta) - 6)
                    Case "0"
                        BotonesRepeticion_Click(btNormal, EventArgs.Empty)
                    Case "1"
                        BotonesRepeticion_Click(btEqPop, EventArgs.Empty)
                    Case "2"
                        BotonesRepeticion_Click(btEqRock, EventArgs.Empty)
                    Case "3"
                        BotonesRepeticion_Click(btEqJazz, EventArgs.Empty)
                    Case "4"
                        BotonesRepeticion_Click(btEqClassic, EventArgs.Empty)
                End Select

        End Select



    End Sub

    Private Sub btExpulsarSD_Click(sender As Object, e As EventArgs) Handles btExpulsarSD.Click

        If unidadMP3 <> "" Then
            ultimoMp3 = 0
            listaMP3.AllowDrop = False
            cerrarDB(False)

            ' Esperar para que Windows libere el archivo.
            ' Necesario por un bug conocido en Microsoft.Data.Sqlite[citation:9]
            Threading.Thread.Sleep(500)

            ExpulsarMicroSD(unidadMP3)

            LblEspacio.Text = "Conectar microSD"
        End If

    End Sub

    Public Sub ActualizarBarraEspacio(driveLetter As String, progressBar As System.Windows.Forms.ProgressBar, Optional label As Label = Nothing)
        Try
            Dim drive As New IO.DriveInfo(driveLetter)

            If Not drive.IsReady Then
                MessageBox.Show("La unidad no está lista.", "Aviso", MessageBoxButtons.OK, MessageBoxIcon.Warning)
                Return
            End If

            Dim totalBytes As Long = drive.TotalSize
            Dim usedBytes As Long = totalBytes - drive.AvailableFreeSpace

            Dim porcentajeUsado As Integer = CInt((usedBytes / totalBytes) * 100)

            progressBar.Minimum = 0
            progressBar.Maximum = 100
            progressBar.Value = porcentajeUsado

            If porcentajeUsado >= 90 Then
                progressBar.ForeColor = Color.Red
            ElseIf porcentajeUsado >= 70 Then
                progressBar.ForeColor = Color.Orange
            Else
                progressBar.ForeColor = Color.Green
            End If

            If label IsNot Nothing Then
                label.Text = $"{FormatBytes(usedBytes)} / {FormatBytes(totalBytes)} ({porcentajeUsado}%)"
            End If

        Catch ex As IO.DriveNotFoundException
            MessageBox.Show($"Unidad '{driveLetter}' no encontrada.", "Error", MessageBoxButtons.OK, MessageBoxIcon.Error)
        Catch ex As Exception
            MessageBox.Show($"Error: {ex.Message}", "Error", MessageBoxButtons.OK, MessageBoxIcon.Error)
        End Try
    End Sub

    ' Convierte bytes a KB, MB o GB legibles
    Private Function FormatBytes(bytes As Long) As String
        If bytes >= 1073741824L Then
            Return $"{bytes / 1073741824.0:F1} GB"
        ElseIf bytes >= 1048576L Then
            Return $"{bytes / 1048576.0:F1} MB"
        Else
            Return $"{bytes / 1024.0:F1} KB"
        End If
    End Function

    ' Convierte la máscara de bits a letra de unidad
    Private Function MaskToDriveLetter(mask As Integer) As String
        For i As Integer = 0 To 25
            If (mask And (1 << i)) <> 0 Then
                Return Chr(Asc("A") + i) & ":"
            End If
        Next
        Return Nothing
    End Function

    Protected Overrides Sub WndProc(ByRef m As Message)
        If m.Msg = WM_DEVICECHANGE Then
            Dim eventCode = m.WParam.ToInt32()

            If eventCode = DBT_DEVICEARRIVAL OrElse
               eventCode = DBT_DEVICEREMOVECOMPLETE Then

                Dim vol = Marshal.PtrToStructure(Of DEV_BROADCAST_VOLUME)(m.LParam)

                ' Solo nos interesan volúmenes (tarjetas SD, USB...)
                If vol.dbcv_devicetype = DBT_DEVTYP_VOLUME Then
                    Dim letra = MaskToDriveLetter(vol.dbcv_unitmask)

                    If eventCode = DBT_DEVICEARRIVAL Then
                        'se ha insertado una tarjeta o usb

                        MsgBox("Para extraer la tarjeta, utilice el botón de 'Expulsar microSD'", MsgBoxStyle.Information, "Atención...")

                        Task.Run(Async Function()
                                     Await Task.Delay(1000)
                                 End Function)

                        UnidadConectada(letra)
                        abrirDB(letra)
                        obtenerMP3()
                        crearRutaMP3(letra)

                    Else

                        'extracción de tarjeta o usb
                        ' ya es demasiado tarde...
                        If letra = unidadMP3 Then
                            MsgBox("! Ha extraído la tarjeta sin expulsarla sin utilizar el botón de 'Expulsar microSD !", MsgBoxStyle.Exclamation, "Atención...")
                        End If

                    End If
                End If
            End If
        End If

        MyBase.WndProc(m)
    End Sub

    Private Sub UnidadConectada(letra As String)
        ' Comprobar que es removible (SD, USB...)
        Try
            Dim drive As New IO.DriveInfo(letra)
            If drive.DriveType = IO.DriveType.Removable Then

                ActualizarBarraEspacio(letra, CapacidadSD, LblEspacio)
            End If
        Catch ex As Exception
            ' La unidad puede tardar en estar lista
        End Try

        unidadMP3 = letra.Substring(0, 1)

    End Sub

    Private Sub UnidadDesconectada(letra As String)
        ' Limpiar interfaz
        CapacidadSD.Value = 0
        LblEspacio.Text = "Sin unidad"
        unidadMP3 = ""
    End Sub

    Private Sub gbControlMesa_Paint(sender As Object, e As PaintEventArgs) Handles gbControlMesa.Paint

        Using pen As New Pen(Color.FromArgb(80, 80, 80), 2)  ' Gris medio-oscuro
            ' Línea vertical: X = 150, desde Y = 20 hasta Y = GroupBox1.Height - 20
            'e.Graphics.DrawLine(pen, 150, 20, 150, GroupBox1.Height - 25)
        End Using
    End Sub

    Sub abrirDB(letra As String)

        Dim base As String = String.Concat(letra, "\", nombreDB, ".db")

        cadenaConexion = String.Concat("Data Source=", base)
        conexion.ConnectionString = cadenaConexion
        conexion.ConnectionString = cadenaConexion

        If Not IO.File.Exists(base) Then
            crearDB(base) 'crea y abre la base de datos
        End If

        conexion.Open()

        ' Desactivar journal (log de transacciones)
        Using cmd As New SqliteCommand("PRAGMA journal_mode = OFF", conexion)
            cmd.ExecuteNonQuery()
        End Using

        ' Desactivar sincronización
        Using cmd As New SqliteCommand("PRAGMA synchronous = OFF", conexion)
            cmd.ExecuteNonQuery()
        End Using


        consultarUltimoMP3()

        listaMP3.AllowDrop = True


    End Sub

    Sub cerrarDB(preguntar As Boolean)

        If preguntar Then
            Dim r As Integer
            r = MsgBox("¿ Seguro que quiere cerrar la base de datos ?", MsgBoxStyle.Question + MsgBoxStyle.YesNo + MsgBoxStyle.DefaultButton2, "Atención...")
            Stop
        End If

        If conexion IsNot Nothing AndAlso conexion.State = ConnectionState.Open Then
            conexion.Close()
            conexion.Dispose()
            conexion = Nothing
            cadenaConexion = ""
            listaMP3.Items.Clear()
        End If

    End Sub

    Async Sub crearDB(base As String)

        Await conexion.OpenAsync()

        Using cmd As New SqliteCommand("
                CREATE TABLE configuracion (
                    version INTEGER DEFAULT 1,
                    pendiente INTEGER DEFAULT 0   -- 0 = false, 1 = true
                )", conexion)
            Await cmd.ExecuteNonQueryAsync()
        End Using

        Using cmd As New SqliteCommand("
                CREATE TABLE mp3 (
                    id INTEGER PRIMARY KEY AUTOINCREMENT,
                    pista INTEGER, 
                    titulo TEXT NOT NULL,
                    artista TEXT NOT NULL,
                    album TEXT,
                    genero TEXT,
                    duracion INTEGER,
                    favorito INTEGER DEFAULT 0,   -- 0 = false, 1 = true
                    bpm REAL,
                    hash_archivo TEXT UNIQUE, 
                    hash_metadatos TEXT,
                    fecha TEXT DEFAULT CURRENT_TIMESTAMP,
                    miniatura BLOB
                )", conexion)
            Await cmd.ExecuteNonQueryAsync()
        End Using

        conexion.Close()

    End Sub

    Sub obtenerMP3()

        listaMP3.Items.Clear()

        Dim sql As String = "select * from mp3 order by pista"

        Using command As New SqliteCommand(sql, conexion)
            Using reader As SqliteDataReader = command.ExecuteReader()
                While reader.Read()

                    'listaMP3.Items.Add(reader("titulo").ToString())

                    Dim itemTexto As String = $"{reader("titulo")} - {reader("artista")}"
                    listaMP3.Items.Add(itemTexto)

                End While
            End Using
        End Using

    End Sub

    Private Sub btCerrarDB_Click(sender As Object, e As EventArgs) Handles btCerrarDB.Click
        cerrarDB(True)
    End Sub

    Private Sub listaMP3_DragEnter(sender As Object, e As DragEventArgs) Handles listaMP3.DragEnter
        If e.Data.GetDataPresent(DataFormats.FileDrop) Then
            e.Effect = DragDropEffects.Copy
        Else
            e.Effect = DragDropEffects.None
        End If
    End Sub

    Private Async Sub listaMP3_DragDrop(sender As Object, e As DragEventArgs) Handles listaMP3.DragDrop

        If _procesandoDragDrop Then Exit Sub
        _procesandoDragDrop = True

        'RemoveHandler listaMP3.DragDrop, AddressOf listaMP3_DragDrop

        Dim rutas() As String = CType(e.Data.GetData(DataFormats.FileDrop), String())

        listaMP3.Enabled = False
        lblEstado.Visible = True

        With progresoMP3
            .Value = 0
            .Visible = True
            .Maximum = rutas.Count
        End With

        ' Ejecutar en segundo plano para no bloquear la UI
        Await Task.Run(Sub()
                           For Each ruta As String In rutas

                               progresoMP3.Invoke(Sub()
                                                      progresoMP3.Value = progresoMP3.Value + 1
                                                      progresoMP3.Refresh()
                                                  End Sub)

                               If IO.File.Exists(ruta) Then
                                   ' Agregar archivo suelto
                                   AgregarAlListBox(ruta)

                               ElseIf IO.Directory.Exists(ruta) Then
                                   ' Procesar carpeta recursivamente
                                   ProcesarCarpetaRecursivaTask(ruta)
                               End If
                           Next
                       End Sub)

        ' Reactivar UI
        listaMP3.Enabled = True

        progresoMP3.Value = 0
        progresoMP3.Visible = False

        lblEstado.Visible = False
        lblTotal.Text = $"Total: {listaMP3.Items.Count} archivos"

        'AddHandler listaMP3.DragDrop, AddressOf listaMP3_DragDrop

        _procesandoDragDrop = False

    End Sub

    Private Sub ProcesarCarpetaRecursivaTask(rutaCarpeta As String)
        Try
            ' Procesar archivos
            Dim archivos() As String = IO.Directory.GetFiles(rutaCarpeta)
            For Each archivo As String In archivos
                AgregarAlListBox(archivo)
            Next

            ' Procesar subcarpetas recursivamente
            Dim subCarpetas() As String = IO.Directory.GetDirectories(rutaCarpeta)
            For Each subCarpeta As String In subCarpetas
                ProcesarCarpetaRecursivaTask(subCarpeta)
            Next

        Catch ex As UnauthorizedAccessException
            ' Ignorar
        End Try
    End Sub

    Private Sub AgregarAlListBox(ruta As String)

        Debug.Print(ruta)

        Dim _destino As String = ""

        If Not esMP3(ruta) Then
            MsgBox($"El fichero '{ruta}' no es un fichero mp3 válido.", MsgBoxStyle.Information, "Atención...")
            Exit Sub
        End If

        'leer metadatos
        Dim datos = LeerDatosMP3(ruta)

        Dim titulo As String = datos.Titulo
        Dim artista As String = datos.Artista
        Dim album As String = datos.Album
        Dim genero As String = datos.Genero
        Dim duracion As Integer = datos.DuracionSegundos
        Dim bpm As Single = datos.BPM
        Dim miniatura = ImageToBytes(GetMp3Thumbnail(ruta))

        Dim nombreLista As String = String.Concat(titulo, " - ", artista)

        'If listaMP3.InvokeRequired Then
        'listaMP3.Invoke(Sub() listaMP3.Items.Add(nombreLista))
        'Else
        'listaMP3.Items.Add(nombreLista)
        'End If

        BeginInvoke(Sub() 'sirve para los dos casos
                        listaMP3.Items.Add(nombreLista)
                    End Sub)



        ultimoMp3 = ultimoMp3 + 1


        'añadir registro
        Dim sql As String = "insert into mp3 (pista, titulo, artista, album, genero, duracion, bpm, hash_archivo, hash_metadatos, miniatura) 
                             VALUES 
                             (@pista, @titulo, @artista, @album, @genero, @duracion, @bpm, @hash_archivo, @hash_metadatos, @miniatura)"

        Using command As New SqliteCommand(sql, conexion)
            ' Agregar los parámetros con sus valores
            command.Parameters.AddWithValue("@pista", ultimoMp3)
            command.Parameters.AddWithValue("@titulo", titulo)
            command.Parameters.AddWithValue("@artista", artista)
            command.Parameters.AddWithValue("@album", album)
            command.Parameters.AddWithValue("@genero", genero)
            command.Parameters.AddWithValue("@duracion", duracion)
            command.Parameters.AddWithValue("@bpm", bpm)
            command.Parameters.AddWithValue("@hash_archivo", hash_archivo(ruta))
            command.Parameters.AddWithValue("@hash_metadatos", hash_metadatos(titulo, artista, album, genero, duracion))
            command.Parameters.AddWithValue("@miniatura", If(miniatura, DBNull.Value))

            ' Ejecutar el INSERT
            Dim _numeroRegistros As Integer = command.ExecuteNonQuery()

            If _numeroRegistros > 0 Then

                'mostrar imagen
                imgCaratula.Image = BytesToImage(miniatura) ' GetMp3Thumbnail(ruta)

                Me.Invoke(Sub()
                              imgCaratula.Refresh()
                              listaMP3.Refresh()
                          End Sub)


                'copiar fichero
                _destino = String.Concat(unidadMP3, ":\", directorioMP3, "\", ultimoMp3.ToString("0000"), " ", titulo, ".mp3")
                IO.File.Copy(ruta, _destino)

                Me.Invoke(Sub()
                              ActualizarBarraEspacio(String.Concat(unidadMP3, ":"), CapacidadSD, LblEspacio)
                          End Sub)

            Else
                MessageBox.Show("No se pudo guardar el registro")
            End If
        End Using


    End Sub

    Sub consultarUltimoMP3()

        Dim sql As String = "select coalesce(max(pista),0) from mp3"
        Using command As New SqliteCommand(sql, conexion)
            Using reader As SqliteDataReader = command.ExecuteReader()
                ' Leer cada fila
                While reader.Read()
                    ' Obtener valores por índice de columna (0 = primera columna)
                    ultimoMp3 = reader.GetInt32(0)
                    ' O también por nombre de columna
                    ' Dim id = Convert.ToInt32(reader("Id"))
                    ' Dim nombre = reader("Nombre").ToString()
                End While
            End Using
        End Using

    End Sub

    Function hash_archivo(filePath As String) As String
        Try
            ' Se abre el archivo en modo de solo lectura y se comparte para evitar conflictos
            Using stream As FileStream = IO.File.OpenRead(filePath)
                ' Se crea el objeto para calcular SHA256
                Using sha256 As SHA256 = SHA256.Create()
                    ' ComputeHash devuelve un arreglo de bytes
                    Dim hashBytes As Byte() = sha256.ComputeHash(stream)

                    ' Se convierte el arreglo de bytes a una cadena hexadecimal
                    Dim hashString As New StringBuilder()
                    For Each b As Byte In hashBytes
                        hashString.Append(b.ToString("x2"))
                    Next
                    Return hashString.ToString()
                End Using
            End Using
        Catch ex As Exception
            ' Manejo de errores: archivo no encontrado, sin permisos, etc.
            MessageBox.Show($"Error al calcular el hash del archivo: {ex.Message}", "Error", MessageBoxButtons.OK, MessageBoxIcon.Error)
            Return Nothing
        End Try
    End Function

    Public Function hash_metadatos(titulo As String, artista As String, album As String, genero As String, duracion As Integer) As String

        ' Normalizar valores nulos a cadena vacía
        titulo = If(titulo, "")
        artista = If(artista, "")
        album = If(album, "")
        genero = If(genero, "")

        ' Crear una cadena canónica con los metadatos
        ' Usar un separador que no pueda aparecer en los textos (ej: "|||")
        Dim metadataString As String = $"{titulo}|||{artista}|||{album}|||{genero}|||{duracion}"

        ' Calcular el hash
        Using sha256 As SHA256 = SHA256.Create()
            Dim inputBytes As Byte() = Encoding.UTF8.GetBytes(metadataString)
            Dim hashBytes As Byte() = sha256.ComputeHash(inputBytes)

            ' Convertir a hexadecimal
            Dim hashString As New StringBuilder()
            For Each b As Byte In hashBytes
                hashString.Append(b.ToString("x2"))
            Next

            Return hashString.ToString()
        End Using
    End Function

    Sub crearRutaMP3(letra As String)

        Dim ruta As String = String.Concat(letra, "\", directorioMP3)

        If Not IO.Directory.Exists(ruta) Then
            IO.Directory.CreateDirectory(ruta)

        End If

    End Sub

    Private Sub SkControl1_PaintSurface(sender As Object, e As SKPaintSurfaceEventArgs) Handles SkControl1.PaintSurface

        Dim canvas = e.Surface.Canvas
        Dim cx = e.Info.Width / 2.0F
        Dim cy = e.Info.Height / 2.0F
        Dim radio = Math.Min(cx, cy)
        Dim rotacion = tbRotacion.Value * Math.PI / 180.0

        'canvas.Clear(SKColors.Black)
        canvas.Clear(SKColors.Transparent)

        'forma redonda para que se parezca a una mesa real
        Dim clipPath As New SKPath()
        clipPath.AddCircle(cx, cy, radio)
        canvas.ClipPath(clipPath)
        Using paintBorde As New SKPaint
            paintBorde.Color = SKColors.Red
            paintBorde.StrokeWidth = 2
            paintBorde.IsAntialias = True
            paintBorde.Style = SKPaintStyle.Stroke
            canvas.DrawCircle(cx, cy, radio, paintBorde)
        End Using


        Using paint As New SKPaint

            paint.Color = SKColors.White
            paint.StrokeWidth = 1.5F
            paint.IsAntialias = True
            paint.Style = SKPaintStyle.Stroke

            Dim puntos As New List(Of SKPoint)
            Dim vueltas = tbVueltas.Value
            Dim pasos = tbDensidad.Value

            Select Case cbPatron.SelectedItem?.ToString()

                Case "Espiral"
                    For i = 0 To pasos
                        Dim t = i / pasos
                        Dim r = t
                        Dim angulo = t * vueltas * 2 * Math.PI
                        puntos.Add(New SKPoint(
                            cx + radio * r * Math.Cos(angulo + rotacion),
                            cy + radio * r * Math.Sin(angulo + rotacion)))
                    Next

                Case "Rosa"
                    Dim k = tbVueltas.Value
                    Dim vueltas_rosa = If(k Mod 2 = 0, 1, 2)
                    For i = 0 To pasos
                        Dim angulo = i / pasos * vueltas_rosa * 2 * Math.PI
                        Dim r = Math.Cos(k * angulo)
                        puntos.Add(New SKPoint(
                            cx + radio * r * Math.Cos(angulo + rotacion),
                            cy + radio * r * Math.Sin(angulo + rotacion)))
                    Next

                Case "Lissajous"
                    Dim fase = tbFase.Value * Math.PI / 180
                    For i = 0 To pasos
                        Dim t = i / pasos * 2 * Math.PI
                        Dim px = radio * Math.Sin(tbVueltas.Value * t + fase)
                        Dim py = radio * Math.Sin(t)
                        puntos.Add(New SKPoint(
                            cx + px * Math.Cos(rotacion) - py * Math.Sin(rotacion),
                            cy - (px * Math.Sin(rotacion) + py * Math.Cos(rotacion))))
                    Next

                Case "Epicicloide"
                    Dim radioFijo = tbVueltas.Value
                    Dim radioRodante = tbDistancia.Value
                    Dim escala = radio / (radioFijo + 2 * radioRodante)
                    For i = 0 To pasos
                        Dim t = i / pasos * 2 * Math.PI * radioRodante
                        Dim px = escala * ((radioFijo + radioRodante) * Math.Cos(t) - radioRodante * Math.Cos((radioFijo + radioRodante) / radioRodante * t))
                        Dim py = escala * ((radioFijo + radioRodante) * Math.Sin(t) - radioRodante * Math.Sin((radioFijo + radioRodante) / radioRodante * t))
                        puntos.Add(New SKPoint(
            cx + px * Math.Cos(rotacion) - py * Math.Sin(rotacion),
            cy - (px * Math.Sin(rotacion) + py * Math.Cos(rotacion))))
                    Next

                Case "Hipocicloide"
                    Dim radioFijo = tbVueltas.Value
                    Dim radioRodante = tbDistancia.Value
                    Dim escala = radio / radioFijo
                    For i = 0 To pasos
                        Dim t = i / pasos * 2 * Math.PI * radioRodante
                        Dim px = escala * ((radioFijo - radioRodante) * Math.Cos(t) + radioRodante * Math.Cos((radioFijo - radioRodante) / radioRodante * t))
                        Dim py = escala * ((radioFijo - radioRodante) * Math.Sin(t) - radioRodante * Math.Sin((radioFijo - radioRodante) / radioRodante * t))
                        puntos.Add(New SKPoint(
            cx + px * Math.Cos(rotacion) - py * Math.Sin(rotacion),
            cy - (px * Math.Sin(rotacion) + py * Math.Cos(rotacion))))
                    Next

                Case "Espiral logarítmica"
                    Dim crecimiento = tbVueltas.Value / 10.0
                    Dim vueltas_Espiral = tbDistancia.Value
                    For i = 0 To pasos
                        Dim t = i / pasos * 2 * Math.PI * vueltas_Espiral
                        Dim r = Math.Exp(crecimiento * t) - 1
                        Dim rMax = Math.Exp(crecimiento * 2 * Math.PI * vueltas_Espiral) - 1
                        Dim px = radio * (r / rMax) * Math.Cos(t)
                        Dim py = radio * (r / rMax) * Math.Sin(t)
                        puntos.Add(New SKPoint(
            cx + px * Math.Cos(rotacion) - py * Math.Sin(rotacion),
            cy - (px * Math.Sin(rotacion) + py * Math.Cos(rotacion))))
                    Next

                Case "Espiral de Fermat"
                    Dim vueltasFermat = tbVueltas.Value
                    For i = 0 To pasos
                        Dim t = i / pasos * 2 * Math.PI * vueltasFermat
                        Dim r = Math.Sqrt(t / (2 * Math.PI * vueltasFermat))
                        Dim px = radio * r * Math.Cos(t)
                        Dim py = radio * r * Math.Sin(t)
                        puntos.Add(New SKPoint(
            cx + px * Math.Cos(rotacion) - py * Math.Sin(rotacion),
            cy - (px * Math.Sin(rotacion) + py * Math.Cos(rotacion))))
                    Next

                Case "Círculos concéntricos"
                    Dim numCirculos = tbVueltas.Value
                    For c = 1 To numCirculos
                        Dim r = c / numCirculos
                        For i = 0 To pasos
                            Dim t = i / pasos * 2 * Math.PI
                            Dim px = radio * r * Math.Cos(t)
                            Dim py = radio * r * Math.Sin(t)
                            puntos.Add(New SKPoint(
                cx + px * Math.Cos(rotacion) - py * Math.Sin(rotacion),
                cy - (px * Math.Sin(rotacion) + py * Math.Cos(rotacion))))
                        Next
                    Next

                Case "Estrella"
                    Dim puntas = tbVueltas.Value
                    Dim profundidad = tbDistancia.Value / 100.0
                    Dim anguloPunta = Math.PI / puntas ' ángulo entre punta exterior e interior

                    For i = 0 To puntas - 1
                        ' Vértice exterior
                        Dim anguloExt = i * 2 * Math.PI / puntas + rotacion
                        puntos.Add(New SKPoint(
            cx + radio * Math.Cos(anguloExt),
            cy - radio * Math.Sin(anguloExt)))

                        ' Vértice interior
                        Dim anguloInt = anguloExt + anguloPunta
                        puntos.Add(New SKPoint(
            cx + radio * profundidad * Math.Cos(anguloInt),
            cy - radio * profundidad * Math.Sin(anguloInt)))
                    Next

                    ' Cerrar la estrella
                    Dim anguloFinal = rotacion
                    puntos.Add(New SKPoint(
        cx + radio * Math.Cos(anguloFinal),
        cy - radio * Math.Sin(anguloFinal)))


                Case "Girasol (Fibonacci)"
                    Dim phi = (1 + Math.Sqrt(5)) / 2
                    Dim anguloAureo = 2 * Math.PI / (phi * phi)
                    Dim escala = tbVueltas.Value ' controla el tamaño del patrón

                    For i = 0 To pasos
                        Dim r = Math.Sqrt(i / CSng(pasos))
                        Dim angulo = i * anguloAureo * escala + rotacion
                        puntos.Add(New SKPoint(
            cx + radio * r * Math.Cos(angulo),
            cy - radio * r * Math.Sin(angulo)))
                    Next

                Case "Patrón áureo (Fibonacci)"
                    Dim phi = (1 + Math.Sqrt(5)) / 2
                    Dim vueltasFibo = tbVueltas.Value

                    For i = 0 To pasos
                        Dim t = i / pasos * 2 * Math.PI * vueltasFibo
                        Dim r = Math.Pow(phi, t / Math.PI)
                        Dim rMax = Math.Pow(phi, 2 * Math.PI * vueltasFibo / Math.PI)
                        puntos.Add(New SKPoint(
                            cx + radio * (r / rMax) * Math.Cos(t + rotacion),
                            cy - radio * (r / rMax) * Math.Sin(t + rotacion)))
                    Next

                Case "Curva de Hilbert"
                    Dim nivel = tbVueltas.Value
                    Dim n = CInt(Math.Pow(2, nivel))

                    ' Calcular primer punto de Hilbert
                    Dim hx0 As Integer, hy0 As Integer
                    HilbertD2XY(n, 0, hx0, hy0)
                    Dim px0 = cx - radio + (2 * radio * hx0 / (n - 1))
                    Dim py0 = cy - radio + (2 * radio * hy0 / (n - 1))
                    Dim anguloInicio = Math.Atan2(py0 - cy, px0 - cx)

                    ' Círculo perimetral empezando desde el ángulo del primer punto de Hilbert
                    For i = 0 To 360
                        Dim t = anguloInicio + i / 360.0 * 2 * Math.PI
                        puntos.Add(New SKPoint(
            cx + radio * Math.Cos(t),
            cy + radio * Math.Sin(t)))
                    Next

                    ' Curva de Hilbert
                    For i = 0 To n * n - 1
                        Dim hx As Integer, hy As Integer
                        HilbertD2XY(n, i, hx, hy)
                        Dim px = cx - radio + (2 * radio * hx / (n - 1))
                        Dim py = cy - radio + (2 * radio * hy / (n - 1))
                        puntos.Add(New SKPoint(px, py))
                    Next


                Case "Moiré"
                    Dim vueltas1 = tbVueltas.Value
                    Dim vueltas2 = vueltas1 + tbDistancia.Value / 10.0 ' ligero desfase

                    ' Primera espiral
                    For i = 0 To pasos
                        Dim t = i / pasos * 2 * Math.PI * vueltas1
                        Dim r = t / (2 * Math.PI * vueltas1)
                        puntos.Add(New SKPoint(
                            cx + radio * r * Math.Cos(t + rotacion),
                            cy - radio * r * Math.Sin(t + rotacion)))
                    Next

                    ' Segunda espiral ligeramente diferente
                    For i = 0 To pasos
                        Dim t = i / pasos * 2 * Math.PI * vueltas2
                        Dim r = t / (2 * Math.PI * vueltas2)
                        puntos.Add(New SKPoint(
                            cx + radio * r * Math.Cos(t + rotacion),
                            cy - radio * r * Math.Sin(t + rotacion)))
                    Next


                Case "Espiral doble"
                    Dim vueltasEspiral = tbVueltas.Value

                    ' Primer brazo
                    For i = 0 To pasos
                        Dim t = i / pasos * 2 * Math.PI * vueltasEspiral
                        Dim r = t / (2 * Math.PI * vueltasEspiral)
                        puntos.Add(New SKPoint(
                            cx + radio * r * Math.Cos(t + rotacion),
                            cy - radio * r * Math.Sin(t + rotacion)))
                    Next

                    ' Segundo brazo desfasado 180°
                    For i = 0 To pasos
                        Dim t = i / pasos * 2 * Math.PI * vueltasEspiral
                        Dim r = t / (2 * Math.PI * vueltasEspiral)
                        puntos.Add(New SKPoint(
                            cx + radio * r * Math.Cos(t + rotacion + Math.PI),
                            cy - radio * r * Math.Sin(t + rotacion + Math.PI)))
                    Next


                Case "Trocoide"
                    Dim radioRueda = tbVueltas.Value
                    Dim distancia = tbDistancia.Value / 10.0 ' distancia del punto al centro
                    Dim vueltasTrocoide = 5

                    For i = 0 To pasos
                        Dim t = i / pasos * 2 * Math.PI * vueltasTrocoide
                        Dim px = radio / (radioRueda + distancia) * (radioRueda * t - distancia * Math.Sin(t))
                        Dim py = radio / (radioRueda + distancia) * (radioRueda - distancia * Math.Cos(t))

                        ' Centrar y aplicar rotación
                        Dim pxC = px - radio / 2
                        Dim pyC = py - radio / (radioRueda + distancia) * radioRueda
                        puntos.Add(New SKPoint(
                            cx + pxC * Math.Cos(rotacion) - pyC * Math.Sin(rotacion),
                            cy - (pxC * Math.Sin(rotacion) + pyC * Math.Cos(rotacion))))
                    Next


                Case "Cardioide"
                    Dim k = tbVueltas.Value
                    For i = 0 To pasos
                        Dim t = i / pasos * 2 * Math.PI
                        Dim r = 1 - Math.Cos(k * t)
                        Dim px = radio / 2 * r * Math.Cos(t)
                        Dim py = radio / 2 * r * Math.Sin(t)
                        puntos.Add(New SKPoint(
                            cx + px * Math.Cos(rotacion) - py * Math.Sin(rotacion),
                            cy - (px * Math.Sin(rotacion) + py * Math.Cos(rotacion))))
                    Next

                Case "Mariposa"
                    Dim k = tbVueltas.Value

                    ' Calcular rMax dinámicamente
                    Dim rMax = 0.0
                    For i = 0 To pasos
                        Dim t = i / pasos * 4 * Math.PI
                        Dim r = Math.Exp(Math.Sin(t)) - 2 * Math.Cos(k * t) + Math.Pow(Math.Sin((2 * t - Math.PI) / 24), 5)
                        If r > rMax Then rMax = r
                    Next

                    ' Dibujar con rMax correcto
                    For i = 0 To pasos
                        Dim t = i / pasos * 4 * Math.PI
                        Dim r = Math.Exp(Math.Sin(t)) - 2 * Math.Cos(k * t) + Math.Pow(Math.Sin((2 * t - Math.PI) / 24), 5)
                        Dim px = radio * (r / rMax) * Math.Cos(t)
                        Dim py = radio * (r / rMax) * Math.Sin(t)
                        puntos.Add(New SKPoint(
            cx + px * Math.Cos(rotacion) - py * Math.Sin(rotacion),
            cy - (px * Math.Sin(rotacion) + py * Math.Cos(rotacion))))
                    Next


                Case "Áster"
                    Dim puntas = tbVueltas.Value
                    Dim k = tbDistancia.Value / 10.0 ' suavidad de las puntas

                    For i = 0 To pasos
                        Dim t = i / pasos * 2 * Math.PI
                        Dim r = 1 + k * Math.Cos(puntas * t)
                        Dim rMax = 1 + k
                        Dim px = radio * (r / rMax) * Math.Cos(t)
                        Dim py = radio * (r / rMax) * Math.Sin(t)
                        puntos.Add(New SKPoint(
                            cx + px * Math.Cos(rotacion) - py * Math.Sin(rotacion),
                            cy - (px * Math.Sin(rotacion) + py * Math.Cos(rotacion))))
                    Next


            End Select


            For i = 0 To puntos.Count - 2
                canvas.DrawLine(puntos(i), puntos(i + 1), paint)
            Next

        End Using

    End Sub

    Private Sub ActualizarVista()

        Select Case cbPatron.SelectedItem?.ToString()
            Case "Espiral"
                lblVueltas.Text = $"Vueltas: {tbVueltas.Value}"
            Case "Rosa"
                lblVueltas.Text = $"Pétalos: {tbVueltas.Value}"
            Case "Lissajous"
                lblVueltas.Text = $"Ratio: {tbVueltas.Value}"
            Case "Epicicloide", "Hipocicloide"
                lblVueltas.Text = $"Radio fijo: {tbVueltas.Value}"
            Case "Espiral logarítmica"
                lblVueltas.Text = $"Crecimiento: {tbVueltas.Value}"
            Case "Curva de Hilbert"
                lblVueltas.Text = $"Nivel: {tbVueltas.Value}"
            Case "Cardioide"
                lblVueltas.Text = $"k: {tbVueltas.Value}"
        End Select

        Select Case cbPatron.SelectedItem?.ToString()
            Case "Epicicloide", "Hipocicloide"
                lblDistancia.Text = $"Radio rodante: {tbDistancia.Value}"
            Case "Espiral logarítmica"
                lblDistancia.Text = $"Vueltas: {tbDistancia.Value}"
        End Select

        lblDensidad.Text = $"Densidad: {tbDensidad.Value}"
        lblFase.Text = $"Fase: {tbFase.Value}°"
        lblRotacion.Text = $"Rotación: {tbRotacion.Value}°"

        SkControl1.Invalidate() ' fuerza el redibujado

    End Sub

    Private Sub tbVueltas_Scroll(sender As Object, e As EventArgs) Handles tbVueltas.Scroll
        ActualizarVista()
    End Sub

    Private Sub tbDensidad_Scroll(sender As Object, e As EventArgs) Handles tbDensidad.Scroll
        ActualizarVista()
    End Sub

    Private Sub tbDistancia_Scroll(sender As Object, e As EventArgs) Handles tbDistancia.Scroll
        lblDistancia.Text = $"Distancia: {tbDistancia.Value}%"
        ActualizarVista()
    End Sub

    Private Sub tbRotacion_Scroll(sender As Object, e As EventArgs) Handles tbRotacion.Scroll
        lblRotacion.Text = $"Rotación: {tbRotacion.Value}°"
        SkControl1.Invalidate()
    End Sub

    Private Sub tbFase_Scroll(sender As Object, e As EventArgs) Handles tbFase.Scroll
        lblFase.Text = $"Fase: {tbFase.Value}°"
        ActualizarVista()
    End Sub


    Private Sub cbPatron_SelectedIndexChanged(sender As Object, e As EventArgs) Handles cbPatron.SelectedIndexChanged

        Dim patron = cbPatron.SelectedItem?.ToString()

        ' Resetear valores por defecto

        tbFase.Visible = False
        lblFase.Visible = False
        tbDistancia.Visible = False
        lblDistancia.Visible = False
        tbDensidad.Visible = True
        lblDensidad.Visible = True

        tbVueltas.Minimum = 1
        tbVueltas.Maximum = 20
        tbDensidad.Minimum = 10
        tbDensidad.Maximum = 2000

        Select Case patron
            Case "Espiral"
                lblVueltas.Text = $"Vueltas: {tbVueltas.Value}"
                tbVueltas.Maximum = 50

            Case "Rosa"
                lblVueltas.Text = $"Pétalos: {tbVueltas.Value}"
                tbVueltas.Minimum = 0

            Case "Lissajous"
                lblVueltas.Text = $"Ratio: {tbVueltas.Value}"
                tbFase.Visible = True
                lblFase.Visible = True
                tbDensidad.Maximum = 5000

            Case "Polígono"
                lblVueltas.Text = $"Lados: {tbVueltas.Value}"
                tbVueltas.Minimum = 3
                tbDensidad.Maximum = 1000

            Case "Epicicloide"
                lblVueltas.Text = $"Radio fijo: {tbVueltas.Value}"
                tbVueltas.Minimum = 2
                tbDensidad.Minimum = 100
                tbDensidad.Maximum = 3000
                tbDensidad.Value = 1000
                tbDistancia.Visible = True
                lblDistancia.Visible = True
                lblDistancia.Text = $"Radio rodante: {tbDistancia.Value}"

            Case "Hipocicloide"
                lblVueltas.Text = $"Radio fijo: {tbVueltas.Value}"
                tbVueltas.Minimum = 3
                tbDensidad.Minimum = 100
                tbDensidad.Maximum = 3000
                tbDensidad.Value = 1000
                tbDistancia.Visible = True
                lblDistancia.Visible = True
                lblDistancia.Text = $"Radio rodante: {tbDistancia.Value}"

            Case "Espiral logarítmica"
                lblVueltas.Text = $"Crecimiento: {tbVueltas.Value}"
                tbDistancia.Visible = True
                lblDistancia.Visible = True
                tbDistancia.Minimum = 1
                tbDistancia.Maximum = 50
                lblDistancia.Text = $"Vueltas: {tbDistancia.Value}"

            Case "Espiral de Fermat"
                lblVueltas.Text = $"Vueltas: {tbVueltas.Value}"
                tbVueltas.Minimum = 1
                tbVueltas.Maximum = 50
                tbDensidad.Maximum = 2000

            Case "Círculos concéntricos"
                lblVueltas.Text = $"Círculos: {tbVueltas.Value}"
                tbVueltas.Minimum = 1
                tbVueltas.Maximum = 20
                tbDensidad.Maximum = 1000

            Case "Estrella"
                lblVueltas.Text = $"Puntas: {tbVueltas.Value}"
                tbVueltas.Minimum = 3
                tbVueltas.Maximum = 20
                tbDistancia.Visible = True
                lblDistancia.Visible = True
                tbDistancia.Minimum = 1
                tbDistancia.Maximum = 99
                tbDistancia.Value = 50
                lblDistancia.Text = $"Profundidad: {tbDistancia.Value}%"
                tbDensidad.Visible = False
                lblDensidad.Visible = False

            Case "Girasol (Fibonacci)"
                lblVueltas.Text = $"Densidad: {tbVueltas.Value}"
                tbVueltas.Minimum = 1
                tbVueltas.Maximum = 50
                tbDensidad.Maximum = 5000
                tbDensidad.Value = 1000

            Case "Patrón áureo (Fibonacci)"
                lblVueltas.Text = $"Vueltas: {tbVueltas.Value}"
                tbVueltas.Minimum = 1
                tbVueltas.Maximum = 20
                tbDensidad.Maximum = 2000

            Case "Curva de Hilbert"
                lblVueltas.Text = $"Nivel: {tbVueltas.Value}"
                tbVueltas.Minimum = 1
                tbVueltas.Maximum = 6
                tbVueltas.Value = 3
                tbDensidad.Visible = False
                lblDensidad.Visible = False

            Case "Moiré"
                lblVueltas.Text = $"Vueltas: {tbVueltas.Value}"
                tbVueltas.Minimum = 2
                tbVueltas.Maximum = 20
                tbDistancia.Visible = True
                lblDistancia.Visible = True
                tbDistancia.Minimum = 1
                tbDistancia.Maximum = 50
                tbDistancia.Value = 10
                lblDistancia.Text = $"Desfase: {tbDistancia.Value / 10.0:F1}"
                tbDensidad.Maximum = 3000

            Case "Espiral doble"
                lblVueltas.Text = $"Vueltas: {tbVueltas.Value}"
                tbVueltas.Minimum = 1
                tbVueltas.Maximum = 50
                tbDensidad.Maximum = 3000

            Case "Trocoide"
                lblVueltas.Text = $"Radio rueda: {tbVueltas.Value}"
                tbVueltas.Minimum = 1
                tbVueltas.Maximum = 20
                tbDistancia.Visible = True
                lblDistancia.Visible = True
                tbDistancia.Minimum = 1
                tbDistancia.Maximum = 100
                tbDistancia.Value = 50
                lblDistancia.Text = $"Distancia: {tbDistancia.Value / 10.0:F1}"
                tbDensidad.Maximum = 3000

            Case "Cardioide"
                lblVueltas.Text = $"Vueltas: {tbVueltas.Value}"
                tbVueltas.Minimum = 1
                tbVueltas.Maximum = 10
                tbDensidad.Maximum = 2000
                tbDistancia.Visible = False
                lblDistancia.Visible = False

            Case "Mariposa"
                lblVueltas.Text = $"k: {tbVueltas.Value}"
                tbVueltas.Minimum = 1
                tbVueltas.Maximum = 10
                tbDensidad.Maximum = 5000
                tbDensidad.Value = 2000
                tbDistancia.Visible = False
                lblDistancia.Visible = False

            Case "Áster"
                lblVueltas.Text = $"Puntas: {tbVueltas.Value}"
                tbVueltas.Minimum = 2
                tbVueltas.Maximum = 20
                tbDistancia.Visible = True
                lblDistancia.Visible = True
                tbDistancia.Minimum = 1
                tbDistancia.Maximum = 50
                tbDistancia.Value = 10
                lblDistancia.Text = $"Amplitud: {tbDistancia.Value / 10.0:F1}"
                tbDensidad.Maximum = 2000



        End Select

        ActualizarVista()

    End Sub

    Private Sub HilbertD2XY(n As Integer, d As Integer, ByRef x As Integer, ByRef y As Integer)
        Dim rx, ry, s, t As Integer
        x = 0 : y = 0 : t = d
        s = 1
        While s < n
            rx = 1 And (t \ 2)
            ry = 1 And (t Xor rx)
            If ry = 0 Then
                If rx = 1 Then
                    x = s - 1 - x
                    y = s - 1 - y
                End If
                Dim temp = x
                x = y
                y = temp
            End If
            x += s * rx
            y += s * ry
            t \= 4
            s *= 2
        End While
    End Sub


    Private Sub cbModoRGB_SelectedIndexChanged(sender As Object, e As EventArgs) Handles cbModoRGB.SelectedIndexChanged
        ' Mostrar/ocultar color según el modo
        Dim necesitaColor = cbModoRGB.SelectedItem?.ToString() = "Sólido" OrElse
                            cbModoRGB.SelectedItem?.ToString() = "Pulso" OrElse
                            cbModoRGB.SelectedItem?.ToString() = "Persecución"

        Try
            ColorPickerControl1.Visible = necesitaColor
        Catch ex As Exception

        End Try


    End Sub

    Private Sub tbVelocidadRGB_Scroll(sender As Object, e As EventArgs) Handles tbVelocidadRGB.Scroll
        lblVelocidadRGB.Text = $"Velocidad: {tbVelocidadRGB.Value}"
    End Sub

    Private Sub tbBrilloRGB_Scroll(sender As Object, e As EventArgs) Handles tbBrilloRGB.Scroll
        lblBrilloRGB.Text = $"Brillo: {tbBrilloRGB.Value}%"
    End Sub

    Private Async Sub btnEnviarRGB_Click(sender As Object, e As EventArgs) Handles btnEnviarRGB.Click

        Dim modo = cbModoRGB.SelectedItem?.ToString().ToUpper()
        modo = modo.Replace("Ó", "O").Replace("É", "E").Replace("Ú", "U") ' sin tildes para el protocolo
        Dim brilloReal = CInt(tbBrilloRGB.Value * 255 / 100)

        Dim cmd As String = ""

        Select Case cbModoRGB.SelectedItem?.ToString()
            Case "Apagado"
                cmd = $"{inicioMensaje}RGB|modo:APAGADO{finMensaje}"
            Case "Sólido"
                cmd = $"{inicioMensaje}RGB|modo:SOLIDO|r:{_colorRGB.R}|g:{_colorRGB.G}|b:{_colorRGB.B}|brillo:{brilloReal}{finMensaje}"
            Case "Arcoíris"
                cmd = $"{inicioMensaje}RGB|modo:ARCOIRIS|velocidad:{tbVelocidadRGB.Value}|brillo:{brilloReal}{finMensaje}"
            Case "Pulso"
                cmd = $"{inicioMensaje}RGB|modo:PULSO|r:{_colorRGB.R}|g:{_colorRGB.G}|b:{_colorRGB.B}|velocidad:{tbVelocidadRGB.Value}|brillo:{brilloReal}{finMensaje}"
            Case "Fuego"
                cmd = $"{inicioMensaje}RGB|modo:FUEGO|velocidad:{tbVelocidadRGB.Value}|brillo:{brilloReal}{finMensaje}"
            Case "Persecución"
                cmd = $"{inicioMensaje}RGB|modo:PERSECUCION|r:{_colorRGB.R}|g:{_colorRGB.G}|b:{_colorRGB.B}|velocidad:{tbVelocidadRGB.Value}|brillo:{brilloReal}{finMensaje}"
        End Select

        ' Enviar brillo por separado
        Dim cmdBrillo = $"{inicioMensaje}RGB|brillo:{tbBrilloRGB.Value}{finMensaje}"

        If _caracteristica Is Nothing Then Return
        Await Bluetooth.EnviarAsync(_caracteristica, cmd)
        Await Bluetooth.EnviarAsync(_caracteristica, cmdBrillo)

        Debug.WriteLine($"RGB: {cmd}")
        Debug.WriteLine($"Brillo: {cmdBrillo}")
    End Sub

    Private Sub btApagarRGB_Click(sender As Object, e As EventArgs) Handles btApagarRGB.Click
        Dim cmd = $"{inicioMensaje}RGB|modo:APAGADO{finMensaje}"
        If _caracteristica Is Nothing Then Return
        ' Await Bluetooth.EnviarAsync(_caracteristica, cmd)
        Debug.WriteLine($"RGB: {cmd}")
        cbModoRGB.SelectedItem = "Apagado"
    End Sub

    Private Sub ColorPickerControl1_ColorChanged(color As Color)
        _colorRGB = color
    End Sub

    Private Sub tbVolumen_ValueChanged(sender As Object, e As EventArgs) Handles tbVolumen.ValueChanged
        lbVolumen.Text = tbVolumen.Value
    End Sub

    Private Sub TxtRecibido_KeyPress(sender As Object, e As KeyPressEventArgs) Handles TxtRecibido.KeyPress
        e.KeyChar = ""
    End Sub

    Private Async Sub tbVolumen_Scroll(sender As Object, e As EventArgs) Handles tbVolumen.Scroll
        Debug.Print("VOL " & tbVolumen.Value.ToString)
        Await enviarBT("VOL " & tbVolumen.Value.ToString)
    End Sub

    Private Sub LiberarBotones(panel As Panel)

        For Each ctrl As Control In panel.Controls

            If TypeOf ctrl Is Button Then
                ctrl.BackColor = Color.Silver
                ctrl.Enabled = True
                ctrl.Top = 3 '10
            End If

        Next

    End Sub

    Private Async Sub PulsarBoton(b As Button, panel As Panel)

        For Each ctrl As Control In panel.Controls

            If TypeOf ctrl Is Button Then

                ctrl.BackColor = Color.Silver
                ctrl.Top = 3 '10

            End If

        Next

        ' Botón hundido
        b.BackColor = Color.Gray
        b.Top = 5 '12

        Await enviarBT("_MP3 PLAY")

    End Sub

    Private Sub btAnterior_Click(sender As Object, e As EventArgs) Handles btAnterior.Click

        If _play Then
            _anterior = True
            btAnterior.BackColor = Color.Gray
            btAnterior.Top = 5 '12
            timerAnteriorSiguiente.Start()

        End If

    End Sub

    Private Sub btPlay_Click(sender As Object, e As EventArgs) Handles btPlay.Click

        _play = True
        PulsarBoton(btPlay, PanelBotones)

    End Sub

    Private Sub btSiguiente_Click(sender As Object, e As EventArgs) Handles btSiguiente.Click

        If _play Then
            _anterior = True
            btSiguiente.BackColor = Color.Gray
            btSiguiente.Top = 5 '12
            timerAnteriorSiguiente.Start()
        End If

    End Sub

    Private Sub btPausa_Click(sender As Object, e As EventArgs) Handles btPausa.Click

        If _play Then

            If _pausa Then
                _pausa = False
                btPausa.BackColor = Color.Silver
                btPausa.Top = 3 '10
            Else
                _pausa = True
                btPausa.BackColor = Color.Gray
                btPausa.Top = 5 '12
            End If

        End If

    End Sub

    Private Sub btStop_Click(sender As Object, e As EventArgs) Handles btStop.Click

        _stop = True
        _play = False
        _pausa = False
        _anterior = False
        _siguiente = False

        btStop.BackColor = Color.Gray
        btStop.Top = 5 '12

        timerAnteriorSiguiente.Start()


    End Sub

    Private Sub timerAnteriorSiguiente_Tick(sender As Object, e As EventArgs) Handles timerAnteriorSiguiente.Tick

        timerAnteriorSiguiente.Stop()

        _anterior = False
        _siguiente = False

        btAnterior.BackColor = Color.Silver
        btAnterior.Top = 3 '10

        btSiguiente.BackColor = Color.Silver
        btSiguiente.Top = 3 '10

        If _pausa Then
            _pausa = False
            btPausa.BackColor = Color.Silver
            btPausa.Top = 3 '10
        End If

        If _stop Then
            _stop = False
            LiberarBotones(PanelBotones)
        End If


    End Sub

    Private Async Sub BotonesRepeticion_Click(sender As Object, e As EventArgs) Handles btNormal.BotonClick,
            btRepetirTodas.BotonClick, btRepetirCancion.BotonClick, btCancionAleatoria.BotonClick

        Dim botonClickeado = DirectCast(sender, BotonIconoTexto)

        Debug.Print($"Botón clickeado: '{botonClickeado.TextoBoton}'")

        'Debug.Print($"ANTES - Activado: {botonClickeado.Activado}")
        'Debug.Print($"ColorFondoActivado: {botonClickeado.ColorFondoActivado}")

        ' Desactivar todos los botones del grupo
        btNormal.Activado = False
        btRepetirTodas.Activado = False
        btRepetirCancion.Activado = False
        btCancionAleatoria.Activado = False

        ' Activar solo el que se clickeó
        botonClickeado.Activado = True

        botonClickeado.Parent.Refresh()  ' Refresca el contenedor padre (GroupBox o Form)
        Refresh()

        ' Aplicar la ecualización según el botón activado
        Select Case botonClickeado.TextoBoton
            Case "Normal"
                Await enviarBT("RP 0")

            Case "Repetir todas"
                Await enviarBT("RP 1")

            Case "Repetir una", "Repetir canción"
                Await enviarBT("RP 2")

            Case "Aleatorio"
                Await enviarBT("RP 3")
        End Select


    End Sub


    Private Async Sub BotonesEQ_Click(sender As Object, e As EventArgs) Handles btEqNormal.BotonClick,
        btEqPop.BotonClick, btEqClassic.BotonClick, btEqRock.BotonClick, btEqJazz.BotonClick, btEqBass.BotonClick

        Dim botonClickeado = DirectCast(sender, BotonIconoTexto)

        ' Desactivar todos los botones del grupo
        btEqNormal.Activado = False
        btEqPop.Activado = False
        btEqClassic.Activado = False
        btEqRock.Activado = False
        btEqJazz.Activado = False
        btEqClassic.Activado = False
        btEqBass.Activado = False

        ' Activar solo el que se clickeó
        botonClickeado.Activado = True

        ' Aplicar la ecualización según el botón activado
        Select Case botonClickeado.TextoBoton
            Case "Normal"
                Await enviarBT("EQ 0")
            Case "Pop"
                Await enviarBT("EQ 1")
            Case "Rock"
                Await enviarBT("EQ 2")
            Case "Jazz"
                Await enviarBT("EQ 3")
            Case "Classic"
                Await enviarBT("EQ 4")
            Case "Bass"
                Await enviarBT("EQ 5")
        End Select
    End Sub


    Private Sub AplicarColoresBotonesPanel(fondo As Color, icono As Color, texto As Color, panel As Panel)

        For Each ctrl As Control In panel.Controls

            If TypeOf ctrl Is BotonIconoTexto Then

                Dim b As BotonIconoTexto = ctrl

                b.ColorFondoActivado = fondo
                b.ColorTextoActivado = texto
                b.ColorIconoActivado = icono

            End If

        Next

    End Sub

    Private Async Sub tbVolumen_MouseUp(sender As Object, e As MouseEventArgs) Handles tbVolumen.MouseUp
        Await enviarBT("VOL " & tbVolumen.Value.ToString)
    End Sub

    Private Sub Form1_Closing(sender As Object, e As CancelEventArgs) Handles Me.FormClosing
        _finder.DetenerBusqueda()
    End Sub

End Class
