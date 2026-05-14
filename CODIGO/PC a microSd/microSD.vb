Imports System.Runtime.InteropServices
Imports Microsoft.Win32.SafeHandles

Module microSD

    ' --- Constantes P/Invoke ---
    Private Const GENERIC_READ As UInteger = &H80000000UI
    Private Const GENERIC_WRITE As UInteger = &H40000000UI
    Private Const FILE_SHARE_READ As UInteger = &H1
    Private Const FILE_SHARE_WRITE As UInteger = &H2
    Private Const OPEN_EXISTING As UInteger = 3
    Private Const FILE_ATTRIBUTE_NORMAL As UInteger = &H80
    Private ReadOnly INVALID_HANDLE_VALUE As IntPtr = New IntPtr(-1)

    Private Const FSCTL_LOCK_VOLUME As UInteger = &H90018
    Private Const FSCTL_DISMOUNT_VOLUME As UInteger = &H90020
    Private Const IOCTL_STORAGE_MEDIA_REMOVAL As UInteger = &H2D4804
    Private Const IOCTL_STORAGE_EJECT_MEDIA As UInteger = &H2D4808

    ' --- Declaraciones P/Invoke ---
    <DllImport("kernel32.dll", SetLastError:=True, CharSet:=CharSet.Auto)>
    Private Function CreateFile(
        lpFileName As String,
        dwDesiredAccess As UInteger,
        dwShareMode As UInteger,
        lpSecurityAttributes As IntPtr,
        dwCreationDisposition As UInteger,
        dwFlagsAndAttributes As UInteger,
        hTemplateFile As IntPtr) As IntPtr
    End Function

    <DllImport("kernel32.dll", SetLastError:=True)>
    Private Function DeviceIoControl(
        hDevice As IntPtr,
        dwIoControlCode As UInteger,
        lpInBuffer As IntPtr,
        nInBufferSize As UInteger,
        lpOutBuffer As IntPtr,
        nOutBufferSize As UInteger,
        ByRef lpBytesReturned As UInteger,
        lpOverlapped As IntPtr) As Boolean
    End Function

    <DllImport("kernel32.dll", SetLastError:=True)>
    Private Function CloseHandle(hObject As IntPtr) As Boolean
    End Function

    ''' <summary>
    ''' Expulsa de forma segura la unidad especificada (p. ej. "E").
    ''' Lanza InvalidOperationException si algo falla.
    ''' </summary>
    ''' <param name="driveLetter">Letra de unidad sin ":" (ej: "E")</param>
    Public Sub ExpulsarMicroSD(unidad As String)

        If String.IsNullOrWhiteSpace(unidad) OrElse unidad.Length <> 1 Then
            Throw New ArgumentException("Indica una sola letra de unidad (ej: ""E"").", NameOf(unidad))
        End If

        Dim volumePath As String = $"\\.\{unidad.ToUpper()}:"
        Dim bytesReturned As UInteger = 0

        ' 1. Abrir handle de volumen
        Dim hVolume As IntPtr = CreateFile(
            volumePath,
            GENERIC_READ Or GENERIC_WRITE,
            FILE_SHARE_READ Or FILE_SHARE_WRITE,
            IntPtr.Zero,
            OPEN_EXISTING,
            FILE_ATTRIBUTE_NORMAL,
            IntPtr.Zero)

        If hVolume = INVALID_HANDLE_VALUE Then
            Throw New InvalidOperationException(
                $"No se puede abrir el volumen '{volumePath}'. " &
                $"Error Win32: {Marshal.GetLastWin32Error()}. " &
                "Asegúrate de ejecutar como Administrador.")
        End If

        Try
            ' 2. Bloquear el volumen (libera escrituras pendientes)
            If Not DeviceIoControl(hVolume, FSCTL_LOCK_VOLUME,
                                   IntPtr.Zero, 0, IntPtr.Zero, 0,
                                   bytesReturned, IntPtr.Zero) Then
                Throw New InvalidOperationException(
                    $"No se pudo bloquear el volumen. " &
                    $"Error Win32: {Marshal.GetLastWin32Error()}. " &
                    "Puede haber archivos abiertos en la unidad.")
            End If

            ' 3. Desmontar el sistema de archivos
            If Not DeviceIoControl(hVolume, FSCTL_DISMOUNT_VOLUME,
                                   IntPtr.Zero, 0, IntPtr.Zero, 0,
                                   bytesReturned, IntPtr.Zero) Then
                Throw New InvalidOperationException(
                    $"No se pudo desmontar el volumen. " &
                    $"Error Win32: {Marshal.GetLastWin32Error()}.")
            End If

            ' 4. Prevenir la eliminación del medio (deshabilitar)
            ' Se pasa un valor 0 para indicar que se permite la remoción
            Dim prevenirRemocion As Boolean = DeviceIoControl(
                hVolume, IOCTL_STORAGE_MEDIA_REMOVAL,
                IntPtr.Zero, 0, IntPtr.Zero, 0,
                bytesReturned, IntPtr.Zero)

            ' 5. Expulsar el medio físicamente
            If Not DeviceIoControl(hVolume, IOCTL_STORAGE_EJECT_MEDIA,
                                   IntPtr.Zero, 0, IntPtr.Zero, 0,
                                   bytesReturned, IntPtr.Zero) Then
                Throw New InvalidOperationException(
                    $"No se pudo expulsar el medio. " &
                    $"Error Win32: {Marshal.GetLastWin32Error()}.")
            End If

        Finally
            ' Siempre cerrar el handle, aunque haya error
            CloseHandle(hVolume)
        End Try

    End Sub

End Module