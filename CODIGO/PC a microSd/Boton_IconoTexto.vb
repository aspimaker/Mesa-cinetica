Imports System.ComponentModel

<DesignerCategory("Code")>
Public Class BotonIconoTexto
    Inherits Panel

    Private lblIcono As Label
    Private lblTexto As Label
    Private _icono As String = ""
    Private _textoBoton As String = ""
    Private _tamañoIcono As Single = 16
    Private _tamañoTexto As Single = 8
    Private _activado As Boolean = False

    ' Colores para estado normal
    Private _colorFondoNormal As Color = SystemColors.Control
    Private _colorTextoNormal As Color = SystemColors.ControlText
    Private _colorIconoNormal As Color = SystemColors.ControlText

    ' Colores para estado activado
    Private _colorFondoActivado As Color = Color.FromArgb(0, 120, 215)
    Private _colorTextoActivado As Color = Color.White
    Private _colorIconoActivado As Color = Color.White

    Public Sub New()
        InitializeComponent()
    End Sub

    Private Sub InitializeComponent()
        lblIcono = New Label()
        lblTexto = New Label()

        Me.SuspendLayout()

        ' Configurar panel
        Me.BackColor = _colorFondoNormal
        Me.BorderStyle = BorderStyle.FixedSingle
        Me.Cursor = Cursors.Hand
        Me.Size = New Size(100, 60)

        ' Icono
        lblIcono.Font = New Font("Segoe UI", _tamañoIcono)
        lblIcono.TextAlign = ContentAlignment.MiddleCenter
        lblIcono.Dock = DockStyle.Top
        lblIcono.Height = 35
        lblIcono.BackColor = Color.Transparent
        lblIcono.ForeColor = _colorIconoNormal
        lblIcono.Text = _icono

        ' Texto
        lblTexto.Font = New Font("Segoe UI", _tamañoTexto)
        lblTexto.TextAlign = ContentAlignment.MiddleCenter
        lblTexto.Dock = DockStyle.Fill
        lblTexto.BackColor = Color.Transparent
        lblTexto.ForeColor = _colorTextoNormal
        lblTexto.Text = _textoBoton

        Me.Controls.Add(lblTexto)
        Me.Controls.Add(lblIcono)

        ' Eventos
        AddHandler Me.Click, AddressOf Boton_Click
        AddHandler lblIcono.Click, AddressOf Boton_Click
        AddHandler lblTexto.Click, AddressOf Boton_Click

        Me.ResumeLayout(False)
    End Sub

    ' ============================================================
    ' PROPIEDAD Text
    ' ============================================================
    <Browsable(True),
     Category("Apariencia"),
     Description("Texto a mostrar en el botón"),
     DefaultValue("")>
    Public Overrides Property Text As String
        Get
            Return _textoBoton
        End Get
        Set(value As String)
            _textoBoton = value
            If lblTexto IsNot Nothing Then
                lblTexto.Text = value
            End If
        End Set
    End Property

    ' ============================================================
    ' PROPIEDAD TextoBoton
    ' ============================================================
    <Browsable(True),
     Category("Apariencia"),
     Description("Texto a mostrar en el botón"),
     DefaultValue("")>
    Public Property TextoBoton As String
        Get
            Return _textoBoton
        End Get
        Set(value As String)
            _textoBoton = value
            If lblTexto IsNot Nothing Then
                lblTexto.Text = value
            End If
        End Set
    End Property

    ' ============================================================
    ' PROPIEDAD Icono
    ' ============================================================
    <Browsable(True),
     Category("Apariencia"),
     Description("Icono a mostrar en el botón"),
     DefaultValue("")>
    Public Property Icono As String
        Get
            Return _icono
        End Get
        Set(value As String)
            _icono = value
            If lblIcono IsNot Nothing Then
                lblIcono.Text = value
            End If
        End Set
    End Property

    ' ============================================================
    ' PROPIEDAD Activado
    ' ============================================================
    <Browsable(True),
     Category("Comportamiento"),
     Description("Indica si el botón está en estado activado/seleccionado"),
     DefaultValue(False)>
    Public Property Activado As Boolean
        Get
            Return _activado
        End Get
        Set(value As Boolean)
            If _activado <> value Then
                _activado = value
                ActualizarColores()
                RaiseEvent ActivadoChanged(Me, EventArgs.Empty)
            End If
        End Set
    End Property

    ' ============================================================
    ' PROPIEDADES DE COLOR - VERSIÓN SIMPLIFICADA SIN ERRORES
    ' ============================================================
    <Browsable(True),
     Category("Apariencia"),
     Description("Color de fondo cuando el botón está activado"),
 DesignerSerializationVisibility(DesignerSerializationVisibility.Hidden)>
    Public Property ColorFondoActivado As Color
        Get
            Return _colorFondoActivado
        End Get
        Set(value As Color)
            _colorFondoActivado = value
            If _activado Then ActualizarColores()
            Me.Invalidate()
        End Set
    End Property

    <Browsable(True),
     Category("Apariencia"),
     Description("Color del texto cuando el botón está activado"),
 DesignerSerializationVisibility(DesignerSerializationVisibility.Hidden)>
    Public Property ColorTextoActivado As Color
        Get
            Return _colorTextoActivado
        End Get
        Set(value As Color)
            _colorTextoActivado = value
            If _activado Then ActualizarColores()
            Me.Invalidate()
        End Set
    End Property

    <Browsable(True),
     Category("Apariencia"),
     Description("Color del icono cuando el botón está activado"),
 DesignerSerializationVisibility(DesignerSerializationVisibility.Hidden)>
    Public Property ColorIconoActivado As Color
        Get
            Return _colorIconoActivado
        End Get
        Set(value As Color)
            _colorIconoActivado = value
            If _activado Then ActualizarColores()
            Me.Invalidate()
        End Set
    End Property

    ' ============================================================
    ' PROPIEDADES DE TAMAÑO
    ' ============================================================
    <Browsable(True),
     Category("Apariencia"),
     Description("Tamaño de fuente del icono"),
     DefaultValue(GetType(Single), "16")>
    Public Property TamañoIcono As Single
        Get
            Return _tamañoIcono
        End Get
        Set(value As Single)
            _tamañoIcono = value
            If lblIcono IsNot Nothing Then
                lblIcono.Font = New Font(lblIcono.Font.FontFamily, value)
                lblIcono.Height = Math.Max(30, CInt(value * 2))
            End If
        End Set
    End Property

    <Browsable(True),
     Category("Apariencia"),
     Description("Tamaño de fuente del texto"),
     DefaultValue(GetType(Single), "8")>
    Public Property TamañoTexto As Single
        Get
            Return _tamañoTexto
        End Get
        Set(value As Single)
            _tamañoTexto = value
            If lblTexto IsNot Nothing Then
                lblTexto.Font = New Font(lblTexto.Font.FontFamily, value)
            End If
        End Set
    End Property

    ' ============================================================
    ' PROPIEDADES BackColor y ForeColor
    ' ============================================================
    <Browsable(True),
     Category("Apariencia")>
    Public Overrides Property BackColor As Color
        Get
            Return _colorFondoNormal
        End Get
        Set(value As Color)
            _colorFondoNormal = value
            If Not _activado Then
                MyBase.BackColor = value
                If lblIcono IsNot Nothing Then lblIcono.BackColor = value
                If lblTexto IsNot Nothing Then lblTexto.BackColor = value
            End If
        End Set
    End Property

    <Browsable(True),
     Category("Apariencia")>
    Public Overrides Property ForeColor As Color
        Get
            Return _colorTextoNormal
        End Get
        Set(value As Color)
            _colorTextoNormal = value
            _colorIconoNormal = value
            If Not _activado Then
                If lblIcono IsNot Nothing Then lblIcono.ForeColor = value
                If lblTexto IsNot Nothing Then lblTexto.ForeColor = value
            End If
        End Set
    End Property

    ' ============================================================
    ' MÉTODO PARA ACTUALIZAR COLORES
    ' ============================================================
    Private Sub ActualizarColores()
        If _activado Then
            MyBase.BackColor = _colorFondoActivado

            If lblIcono IsNot Nothing Then
                lblIcono.ForeColor = _colorIconoActivado
            End If

            If lblTexto IsNot Nothing Then
                lblTexto.ForeColor = _colorTextoActivado
            End If
        Else
            MyBase.BackColor = _colorFondoNormal
            If lblIcono IsNot Nothing Then lblIcono.ForeColor = _colorIconoNormal
            If lblTexto IsNot Nothing Then lblTexto.ForeColor = _colorTextoNormal
        End If

        Me.Invalidate()
        Me.Refresh()

    End Sub

    ' ============================================================
    ' EVENTOS
    ' ============================================================
    Public Event BotonClick As EventHandler
    Public Event ActivadoChanged As EventHandler

    Private Sub Boton_Click(sender As Object, e As EventArgs)
        RaiseEvent BotonClick(Me, e)
    End Sub

    ' ============================================================
    ' EFECTOS VISUALES
    ' ============================================================
    Private _colorBackup As Color

    Protected Overrides Sub OnMouseEnter(e As EventArgs)
        MyBase.OnMouseEnter(e)
        If Not _activado Then
            _colorBackup = MyBase.BackColor
            MyBase.BackColor = SystemColors.ControlLight
        End If
    End Sub

    Protected Overrides Sub OnMouseLeave(e As EventArgs)
        MyBase.OnMouseLeave(e)
        If Not _activado Then
            MyBase.BackColor = _colorFondoNormal
        End If
    End Sub

    Protected Overrides Sub OnEnter(e As EventArgs)
        MyBase.OnEnter(e)
        Me.BorderStyle = BorderStyle.Fixed3D
    End Sub

    Protected Overrides Sub OnLeave(e As EventArgs)
        MyBase.OnLeave(e)
        Me.BorderStyle = BorderStyle.FixedSingle
    End Sub
End Class