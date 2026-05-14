Imports System.Drawing
Imports System.IO
Imports TagLib

Public Class DatosMP3
    Public Property Titulo As String
    Public Property Artista As String
    Public Property Album As String
    Public Property Genero As String
    Public Property DuracionSegundos As Integer
    Public Property BPM As Integer
End Class

Module mp3

    Public Function LeerDatosMP3(rutaFichero As String) As DatosMP3
        Using file = TagLib.File.Create(rutaFichero)
            Dim datos As New DatosMP3()

            datos.Titulo = If(file.Tag.Title, String.Empty)
            datos.Artista = If(file.Tag.FirstPerformer, String.Empty)
            datos.Album = If(file.Tag.Album, String.Empty)
            datos.Genero = If(file.Tag.FirstGenre, String.Empty)
            datos.DuracionSegundos = CInt(file.Properties.Duration.TotalSeconds)
            datos.BPM = CInt(file.Tag.BeatsPerMinute)

            Return datos
        End Using
    End Function

    Public Function GetMp3Thumbnail(filePath As String) As Image

        Try
            Using file = TagLib.File.Create(filePath)

                Dim pictures = file.Tag.Pictures

                If pictures Is Nothing OrElse pictures.Length = 0 Then
                    Return Nothing
                End If

                ' La primera imagen suele ser la carátula frontal
                Dim pic = pictures(0)

                Using ms As New IO.MemoryStream(pic.Data.Data)
                    Return Image.FromStream(ms)
                End Using

            End Using

        Catch ex As Exception
            Return Nothing
        End Try

    End Function

    Public Function ImageToBytes(img As System.Drawing.Image) As Byte()
        If img Is Nothing Then Return Nothing
        Using ms As New IO.MemoryStream()
            img.Save(ms, System.Drawing.Imaging.ImageFormat.Jpeg)
            Return ms.ToArray()
        End Using
    End Function

    Public Function BytesToImage(data As Byte()) As System.Drawing.Image
        If data Is Nothing OrElse data.Length = 0 Then Return Nothing
        Using ms As New IO.MemoryStream(data)
            Return System.Drawing.Image.FromStream(ms)
        End Using
    End Function


    Function esMP3(ruta As String) As Boolean
        ' 1. Verificar que el archivo existe
        If Not IO.File.Exists(ruta) Then Return False

        ' 2. Verificar extensión
        Dim extension As String = Path.GetExtension(ruta).ToLower()
        If extension <> ".mp3" Then Return False

        ' 3. Verificar con TagLib que sea un MP3 válido
        Try
            Using archivo As TagLib.File = TagLib.File.Create(ruta)
                ' Intentar leer una propiedad básica
                Dim bitrate = archivo.Properties.AudioBitrate

                ' Si tiene bitrate > 0, es un audio válido (probablemente MP3)
                Return bitrate > 0
            End Using
        Catch ex As CorruptFileException
            ' Archivo corrupto o no es MP3 válido
            Return False
        Catch ex As UnsupportedFormatException
            ' Formato no soportado por TagLib (no es MP3 válido)
            Return False
        End Try
    End Function

End Module
