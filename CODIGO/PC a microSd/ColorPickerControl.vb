Imports SkiaSharp
Imports SkiaSharp.Views.Desktop

Public Class ColorPickerControl

    Private _skControl As SKControl
    Private _colorSeleccionado As SKColor = SKColors.White
    Private _hue As Single = 0
    Private _saturacion As Single = 1
    Private _brillo As Single = 1
    Private _arrastrando As Boolean = False
    Private _modoArrastre As Integer = 0 ' 1=rueda, 2=triángulo

    Public Event ColorChanged(color As Color)

    Public ReadOnly Property ColorSeleccionado As Color
        Get
            Return Color.FromArgb(_colorSeleccionado.Red, _colorSeleccionado.Green, _colorSeleccionado.Blue)
        End Get
    End Property

    Public Sub New()
        InitializeComponent()
        _skControl = New SKControl()
        _skControl.Dock = DockStyle.Fill
        Controls.Add(_skControl)
        AddHandler _skControl.PaintSurface, AddressOf OnPaintSurface
        AddHandler _skControl.MouseDown, Sub(s, e)
                                             _arrastrando = True
                                             ProcesarClick(e.X, e.Y)
                                         End Sub

        AddHandler _skControl.MouseMove, Sub(s, e)
                                             If _arrastrando Then ProcesarClick(e.X, e.Y)
                                         End Sub

        AddHandler _skControl.MouseUp, Sub(s, e)
                                           _arrastrando = False
                                           _modoArrastre = 0
                                       End Sub
    End Sub

    Private Sub OnPaintSurface(sender As Object, e As SKPaintSurfaceEventArgs)
        Dim canvas = e.Surface.Canvas
        canvas.Clear(SKColors.Transparent)

        Dim cx = e.Info.Width / 2.0F
        Dim cy = e.Info.Height / 2.0F
        Dim radioExterno = Math.Min(cx, cy) * 0.95F
        Dim radioInterno = radioExterno * 0.7F

        ' ── Rueda de colores ──────────────────────────
        Using paint As New SKPaint
            paint.IsAntialias = True
            paint.Style = SKPaintStyle.Stroke
            paint.StrokeWidth = (radioExterno - radioInterno)

            For i = 0 To 359
                paint.Color = SKColor.FromHsv(i, 100, 100)
                Dim angRad = i * Math.PI / 180.0F
                Dim radioMedio = (radioExterno + radioInterno) / 2.0F
                Dim x1 = cx + radioMedio * Math.Cos(angRad)
                Dim y1 = cy + radioMedio * Math.Sin(angRad)
                Dim x2 = cx + radioMedio * Math.Cos((i + 2) * Math.PI / 180.0F)
                Dim y2 = cy + radioMedio * Math.Sin((i + 2) * Math.PI / 180.0F)
                canvas.DrawLine(x1, y1, x2, y2, paint)
            Next
        End Using

        ' ── Triángulo interior ────────────────────────
        Dim r = radioInterno * 0.85F
        Dim p1 = New SKPoint(cx + r * Math.Cos(_hue * Math.PI / 180.0F), cy + r * Math.Sin(_hue * Math.PI / 180.0F))
        Dim p2 = New SKPoint(cx + r * Math.Cos((_hue + 120) * Math.PI / 180.0F), cy + r * Math.Sin((_hue + 120) * Math.PI / 180.0F))
        Dim p3 = New SKPoint(cx + r * Math.Cos((_hue + 240) * Math.PI / 180.0F), cy + r * Math.Sin((_hue + 240) * Math.PI / 180.0F))

        Using shader = SKShader.CreateLinearGradient(
            p1, p2,
            {SKColor.FromHsv(_hue, 100, 100), SKColors.White},
            Nothing, SKShaderTileMode.Clamp)

            Dim path As New SKPath()
            path.MoveTo(p1)
            path.LineTo(p2)
            path.LineTo(p3)
            path.Close()

            Using paint As New SKPaint
                paint.IsAntialias = True
                paint.Shader = shader
                canvas.DrawPath(path, paint)
            End Using
        End Using

        ' Degradado negro encima
        Using shader = SKShader.CreateLinearGradient(
            p3, New SKPoint((p1.X + p2.X) / 2, (p1.Y + p2.Y) / 2),
            {SKColors.Black, SKColors.Transparent},
            Nothing, SKShaderTileMode.Clamp)

            Dim path As New SKPath()
            path.MoveTo(p1)
            path.LineTo(p2)
            path.LineTo(p3)
            path.Close()

            Using paint As New SKPaint
                paint.IsAntialias = True
                paint.Shader = shader
                canvas.DrawPath(path, paint)
            End Using
        End Using

        ' ── Preview color seleccionado ────────────────
        Using paint As New SKPaint
            paint.Color = _colorSeleccionado
            paint.IsAntialias = True
            paint.Style = SKPaintStyle.Fill
            canvas.DrawCircle(cx, cy, radioInterno * 0.3F, paint)
        End Using

        ' ── Indicador en la rueda ─────────────────────
        Using paint As New SKPaint
            paint.Color = SKColors.White
            paint.IsAntialias = True
            paint.Style = SKPaintStyle.Stroke
            paint.StrokeWidth = 2
            Dim radioMedio = (radioExterno + radioInterno) / 2.0F
            Dim angRad = _hue * Math.PI / 180.0F
            canvas.DrawCircle(
                cx + radioMedio * Math.Cos(angRad),
                cy + radioMedio * Math.Sin(angRad),
                6, paint)
        End Using
    End Sub

    Protected Overrides Sub OnMouseDown(e As MouseEventArgs)
        _arrastrando = True
        ProcesarClick(e.X, e.Y)
        MyBase.OnMouseDown(e)
    End Sub

    Protected Overrides Sub OnMouseMove(e As MouseEventArgs)
        If _arrastrando Then ProcesarClick(e.X, e.Y)
        MyBase.OnMouseMove(e)
    End Sub

    Protected Overrides Sub OnMouseUp(e As MouseEventArgs)
        _arrastrando = False
        _modoArrastre = 0
        MyBase.OnMouseUp(e)
    End Sub

    Private Sub ProcesarClick(x As Integer, y As Integer)
        Dim cx = _skControl.Width / 2.0F
        Dim cy = _skControl.Height / 2.0F
        Dim radioExterno = Math.Min(cx, cy) * 0.95F
        Dim radioInterno = radioExterno * 0.7F
        Dim dx = x - cx
        Dim dy = y - cy
        Dim dist = Math.Sqrt(dx * dx + dy * dy)

        If (dist >= radioInterno AndAlso dist <= radioExterno) OrElse _modoArrastre = 1 Then
            ' Click en la rueda → cambiar hue
            _modoArrastre = 1
            _hue = (Math.Atan2(dy, dx) * 180.0F / Math.PI + 360) Mod 360
        ElseIf dist < radioInterno OrElse _modoArrastre = 2 Then
            ' Click en el triángulo → cambiar saturación/brillo
            _modoArrastre = 2
            _saturacion = Math.Min(1, Math.Max(0, dist / (radioInterno * 0.85F)))
            _brillo = Math.Min(1, Math.Max(0, 1 - (dy + radioInterno) / (radioInterno * 2)))
        End If

        _colorSeleccionado = SKColor.FromHsv(_hue, _saturacion * 100, _brillo * 100)
        _skControl.Invalidate()
        RaiseEvent ColorChanged(ColorSeleccionado)
    End Sub

End Class