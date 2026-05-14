Imports Microsoft.Data.Sqlite
Imports TagLib.IFD.Entries

Module Module1

    Public Const inicioMensaje As String = "_"
    Public Const finMensaje As String = "#"
    Public Const nombreDB As String = "aspimaker-kntc"
    Public Const directorioMP3 As String = "mp3"

    Public unidadMP3 As String = ""
    Public ultimoMp3 As Integer = 0

    Public cadenaConexion As String = "Data Source="
    Public conexion As SqliteConnection

    Public TemaOscuro As Boolean = False

    Enum TipoTema
        Claro
        Oscuro
    End Enum


End Module
