<Global.Microsoft.VisualBasic.CompilerServices.DesignerGenerated()>
Partial Class Form1
    Inherits System.Windows.Forms.Form

    'Form overrides dispose to clean up the component list.
    <System.Diagnostics.DebuggerNonUserCode()>
    Protected Overrides Sub Dispose(disposing As Boolean)
        Try
            If disposing AndAlso components IsNot Nothing Then
                components.Dispose()
            End If
        Finally
            MyBase.Dispose(disposing)
        End Try
    End Sub

    'Required by the Windows Form Designer
    Private components As System.ComponentModel.IContainer

    'NOTE: The following procedure is required by the Windows Form Designer
    'It can be modified using the Windows Form Designer.
    'Do not modify it using the code editor.
    <System.Diagnostics.DebuggerStepThrough()>
    Private Sub InitializeComponent()
        components = New ComponentModel.Container()
        Dim resources As System.ComponentModel.ComponentResourceManager = New System.ComponentModel.ComponentResourceManager(GetType(Form1))
        ListBoxDispositivos = New ListBox()
        LabelEstado = New Label()
        BtnBuscar = New Button()
        BtnDetener = New Button()
        ListaMesas = New ListBox()
        BtnConectar = New Button()
        TxtEnviar = New TextBox()
        TxtRecibido = New TextBox()
        BtnEnviar = New Button()
        GroupBox1 = New GroupBox()
        GroupBox2 = New GroupBox()
        gbControlMesa = New GroupBox()
        lbVolumen = New Label()
        GroupBox7 = New GroupBox()
        panelModoRepeticion = New Panel()
        btRepetirTodas = New BotonIconoTexto()
        btCancionAleatoria = New BotonIconoTexto()
        btNormal = New BotonIconoTexto()
        btRepetirCancion = New BotonIconoTexto()
        GroupBox8 = New GroupBox()
        panelEqualizacion = New Panel()
        btEqBass = New BotonIconoTexto()
        btEqNormal = New BotonIconoTexto()
        btEqPop = New BotonIconoTexto()
        btEqClassic = New BotonIconoTexto()
        btEqRock = New BotonIconoTexto()
        btEqJazz = New BotonIconoTexto()
        Label3 = New Label()
        GroupBox6 = New GroupBox()
        PanelBotones = New Panel()
        btPausa = New Button()
        btAnterior = New Button()
        btStop = New Button()
        btPlay = New Button()
        btSiguiente = New Button()
        tbVolumen = New TrackBar()
        gbMP3 = New GroupBox()
        imgCaratula = New PictureBox()
        progresoMP3 = New ProgressBar()
        btExpulsarSD = New Button()
        listaMP3 = New ListBox()
        LblEspacio = New Label()
        CapacidadSD = New ProgressBar()
        btCerrarDB = New Button()
        lblEstado = New Label()
        lblTotal = New Label()
        ImageList1 = New ImageList(components)
        SkControl1 = New SkiaSharp.Views.Desktop.SKControl()
        tbDensidad = New TrackBar()
        cbPatron = New ComboBox()
        lblVueltas = New Label()
        lblFase = New Label()
        tbFase = New TrackBar()
        lblDistancia = New Label()
        tbDistancia = New TrackBar()
        lblRotacion = New Label()
        tbRotacion = New TrackBar()
        gbListaPatrones = New GroupBox()
        ckEnBucle = New CheckBox()
        btAñadirCola = New Button()
        btParar = New Button()
        listaPatrones = New ListBox()
        btIniciarCola = New Button()
        btPausarCola = New Button()
        gbPatrones = New GroupBox()
        lblDensidad = New Label()
        Label1 = New Label()
        tbVueltas = New TrackBar()
        gbRGB = New GroupBox()
        ColorPickerControl1 = New ColorPickerControl()
        btApagarRGB = New Button()
        btnEnviarRGB = New Button()
        lblBrilloRGB = New Label()
        tbBrilloRGB = New TrackBar()
        lblVelocidadRGB = New Label()
        tbVelocidadRGB = New TrackBar()
        Label2 = New Label()
        cbModoRGB = New ComboBox()
        timerAnteriorSiguiente = New Timer(components)
        GroupBox9 = New GroupBox()
        lbCancionesMesa = New ListBox()
        GroupBox1.SuspendLayout()
        GroupBox2.SuspendLayout()
        gbControlMesa.SuspendLayout()
        GroupBox7.SuspendLayout()
        panelModoRepeticion.SuspendLayout()
        GroupBox8.SuspendLayout()
        panelEqualizacion.SuspendLayout()
        GroupBox6.SuspendLayout()
        PanelBotones.SuspendLayout()
        CType(tbVolumen, ComponentModel.ISupportInitialize).BeginInit()
        gbMP3.SuspendLayout()
        CType(imgCaratula, ComponentModel.ISupportInitialize).BeginInit()
        CType(tbDensidad, ComponentModel.ISupportInitialize).BeginInit()
        CType(tbFase, ComponentModel.ISupportInitialize).BeginInit()
        CType(tbDistancia, ComponentModel.ISupportInitialize).BeginInit()
        CType(tbRotacion, ComponentModel.ISupportInitialize).BeginInit()
        gbListaPatrones.SuspendLayout()
        gbPatrones.SuspendLayout()
        CType(tbVueltas, ComponentModel.ISupportInitialize).BeginInit()
        gbRGB.SuspendLayout()
        CType(tbBrilloRGB, ComponentModel.ISupportInitialize).BeginInit()
        CType(tbVelocidadRGB, ComponentModel.ISupportInitialize).BeginInit()
        GroupBox9.SuspendLayout()
        SuspendLayout()
        ' 
        ' ListBoxDispositivos
        ' 
        ListBoxDispositivos.Font = New Font("OCR A Extended", 9F, FontStyle.Regular, GraphicsUnit.Point, CByte(0))
        ListBoxDispositivos.FormattingEnabled = True
        ListBoxDispositivos.Location = New Point(6, 22)
        ListBoxDispositivos.Name = "ListBoxDispositivos"
        ListBoxDispositivos.Size = New Size(231, 290)
        ListBoxDispositivos.TabIndex = 0
        ' 
        ' LabelEstado
        ' 
        LabelEstado.Location = New Point(1259, 520)
        LabelEstado.Name = "LabelEstado"
        LabelEstado.Size = New Size(263, 15)
        LabelEstado.TabIndex = 1
        LabelEstado.Text = "Label1"
        ' 
        ' BtnBuscar
        ' 
        BtnBuscar.Location = New Point(1721, 84)
        BtnBuscar.Name = "BtnBuscar"
        BtnBuscar.Size = New Size(92, 34)
        BtnBuscar.TabIndex = 2
        BtnBuscar.Text = "Buscar"
        BtnBuscar.UseVisualStyleBackColor = True
        BtnBuscar.Visible = False
        ' 
        ' BtnDetener
        ' 
        BtnDetener.Location = New Point(1721, 125)
        BtnDetener.Name = "BtnDetener"
        BtnDetener.Size = New Size(92, 33)
        BtnDetener.TabIndex = 3
        BtnDetener.Text = "Parar"
        BtnDetener.UseVisualStyleBackColor = True
        BtnDetener.Visible = False
        ' 
        ' ListaMesas
        ' 
        ListaMesas.Font = New Font("OCR A Extended", 9F, FontStyle.Regular, GraphicsUnit.Point, CByte(0))
        ListaMesas.FormattingEnabled = True
        ListaMesas.Location = New Point(6, 18)
        ListaMesas.Name = "ListaMesas"
        ListaMesas.Size = New Size(133, 251)
        ListaMesas.TabIndex = 4
        ' 
        ' BtnConectar
        ' 
        BtnConectar.Location = New Point(6, 280)
        BtnConectar.Name = "BtnConectar"
        BtnConectar.Size = New Size(133, 32)
        BtnConectar.TabIndex = 5
        BtnConectar.Text = "Conectar"
        BtnConectar.UseVisualStyleBackColor = True
        ' 
        ' TxtEnviar
        ' 
        TxtEnviar.Location = New Point(1721, 188)
        TxtEnviar.Name = "TxtEnviar"
        TxtEnviar.Size = New Size(145, 23)
        TxtEnviar.TabIndex = 7
        ' 
        ' TxtRecibido
        ' 
        TxtRecibido.Location = New Point(1259, 541)
        TxtRecibido.Name = "TxtRecibido"
        TxtRecibido.Size = New Size(264, 23)
        TxtRecibido.TabIndex = 8
        ' 
        ' BtnEnviar
        ' 
        BtnEnviar.Location = New Point(1721, 215)
        BtnEnviar.Name = "BtnEnviar"
        BtnEnviar.Size = New Size(92, 21)
        BtnEnviar.TabIndex = 9
        BtnEnviar.Text = "Enviar"
        BtnEnviar.UseVisualStyleBackColor = True
        ' 
        ' GroupBox1
        ' 
        GroupBox1.Controls.Add(ListBoxDispositivos)
        GroupBox1.Font = New Font("OCR A Extended", 9F)
        GroupBox1.Location = New Point(10, 11)
        GroupBox1.Name = "GroupBox1"
        GroupBox1.Size = New Size(244, 324)
        GroupBox1.TabIndex = 10
        GroupBox1.TabStop = False
        GroupBox1.Text = "Dispositivos BLE al alcance: "
        ' 
        ' GroupBox2
        ' 
        GroupBox2.Controls.Add(ListaMesas)
        GroupBox2.Controls.Add(BtnConectar)
        GroupBox2.Font = New Font("OCR A Extended", 9F)
        GroupBox2.Location = New Point(260, 11)
        GroupBox2.Name = "GroupBox2"
        GroupBox2.Size = New Size(145, 324)
        GroupBox2.TabIndex = 11
        GroupBox2.TabStop = False
        GroupBox2.Text = "Mesas cinéticas: "
        ' 
        ' gbControlMesa
        ' 
        gbControlMesa.Controls.Add(lbVolumen)
        gbControlMesa.Controls.Add(GroupBox7)
        gbControlMesa.Controls.Add(GroupBox8)
        gbControlMesa.Controls.Add(Label3)
        gbControlMesa.Controls.Add(GroupBox6)
        gbControlMesa.Controls.Add(tbVolumen)
        gbControlMesa.Font = New Font("OCR A Extended", 9F)
        gbControlMesa.Location = New Point(10, 341)
        gbControlMesa.Name = "gbControlMesa"
        gbControlMesa.Size = New Size(691, 223)
        gbControlMesa.TabIndex = 12
        gbControlMesa.TabStop = False
        gbControlMesa.Text = "Reproductor"
        ' 
        ' lbVolumen
        ' 
        lbVolumen.AutoSize = True
        lbVolumen.Location = New Point(655, 33)
        lbVolumen.Name = "lbVolumen"
        lbVolumen.Size = New Size(14, 13)
        lbVolumen.TabIndex = 25
        lbVolumen.Text = "0"
        ' 
        ' GroupBox7
        ' 
        GroupBox7.Controls.Add(panelModoRepeticion)
        GroupBox7.Location = New Point(10, 118)
        GroupBox7.Name = "GroupBox7"
        GroupBox7.Size = New Size(379, 97)
        GroupBox7.TabIndex = 34
        GroupBox7.TabStop = False
        GroupBox7.Text = "Modo repetición: "
        ' 
        ' panelModoRepeticion
        ' 
        panelModoRepeticion.BackColor = Color.Transparent
        panelModoRepeticion.Controls.Add(btRepetirTodas)
        panelModoRepeticion.Controls.Add(btCancionAleatoria)
        panelModoRepeticion.Controls.Add(btNormal)
        panelModoRepeticion.Controls.Add(btRepetirCancion)
        panelModoRepeticion.Location = New Point(6, 15)
        panelModoRepeticion.Name = "panelModoRepeticion"
        panelModoRepeticion.Size = New Size(367, 76)
        panelModoRepeticion.TabIndex = 45
        ' 
        ' btRepetirTodas
        ' 
        btRepetirTodas.BackColor = SystemColors.Control
        btRepetirTodas.BorderStyle = BorderStyle.FixedSingle
        btRepetirTodas.Location = New Point(97, 4)
        btRepetirTodas.Name = "btRepetirTodas"
        btRepetirTodas.Size = New Size(70, 70)
        btRepetirTodas.TabIndex = 43
        ' 
        ' btCancionAleatoria
        ' 
        btCancionAleatoria.BackColor = SystemColors.Control
        btCancionAleatoria.BorderStyle = BorderStyle.FixedSingle
        btCancionAleatoria.Location = New Point(291, 4)
        btCancionAleatoria.Name = "btCancionAleatoria"
        btCancionAleatoria.Size = New Size(70, 70)
        btCancionAleatoria.TabIndex = 44
        ' 
        ' btNormal
        ' 
        btNormal.BackColor = SystemColors.Control
        btNormal.BorderStyle = BorderStyle.FixedSingle
        btNormal.Location = New Point(0, 4)
        btNormal.Name = "btNormal"
        btNormal.Size = New Size(70, 70)
        btNormal.TabIndex = 42
        ' 
        ' btRepetirCancion
        ' 
        btRepetirCancion.BackColor = SystemColors.Control
        btRepetirCancion.BorderStyle = BorderStyle.FixedSingle
        btRepetirCancion.Location = New Point(194, 4)
        btRepetirCancion.Name = "btRepetirCancion"
        btRepetirCancion.Size = New Size(70, 70)
        btRepetirCancion.TabIndex = 43
        ' 
        ' GroupBox8
        ' 
        GroupBox8.Controls.Add(panelEqualizacion)
        GroupBox8.Location = New Point(391, 19)
        GroupBox8.Name = "GroupBox8"
        GroupBox8.Size = New Size(238, 196)
        GroupBox8.TabIndex = 35
        GroupBox8.TabStop = False
        GroupBox8.Text = "Equalización: "
        ' 
        ' panelEqualizacion
        ' 
        panelEqualizacion.Controls.Add(btEqBass)
        panelEqualizacion.Controls.Add(btEqNormal)
        panelEqualizacion.Controls.Add(btEqPop)
        panelEqualizacion.Controls.Add(btEqClassic)
        panelEqualizacion.Controls.Add(btEqRock)
        panelEqualizacion.Controls.Add(btEqJazz)
        panelEqualizacion.Location = New Point(4, 15)
        panelEqualizacion.Name = "panelEqualizacion"
        panelEqualizacion.Size = New Size(228, 175)
        panelEqualizacion.TabIndex = 30
        ' 
        ' btEqBass
        ' 
        btEqBass.BackColor = SystemColors.Control
        btEqBass.BorderStyle = BorderStyle.FixedSingle
        btEqBass.Location = New Point(154, 103)
        btEqBass.Name = "btEqBass"
        btEqBass.Size = New Size(70, 70)
        btEqBass.TabIndex = 42
        ' 
        ' btEqNormal
        ' 
        btEqNormal.BackColor = SystemColors.Control
        btEqNormal.BorderStyle = BorderStyle.FixedSingle
        btEqNormal.Location = New Point(3, 4)
        btEqNormal.Name = "btEqNormal"
        btEqNormal.Size = New Size(70, 70)
        btEqNormal.TabIndex = 35
        ' 
        ' btEqPop
        ' 
        btEqPop.BackColor = SystemColors.Control
        btEqPop.BorderStyle = BorderStyle.FixedSingle
        btEqPop.Location = New Point(78, 4)
        btEqPop.Name = "btEqPop"
        btEqPop.Size = New Size(70, 70)
        btEqPop.TabIndex = 36
        ' 
        ' btEqClassic
        ' 
        btEqClassic.BackColor = SystemColors.Control
        btEqClassic.BorderStyle = BorderStyle.FixedSingle
        btEqClassic.Location = New Point(78, 103)
        btEqClassic.Name = "btEqClassic"
        btEqClassic.Size = New Size(70, 70)
        btEqClassic.TabIndex = 41
        ' 
        ' btEqRock
        ' 
        btEqRock.BackColor = SystemColors.Control
        btEqRock.BorderStyle = BorderStyle.FixedSingle
        btEqRock.Location = New Point(151, 4)
        btEqRock.Name = "btEqRock"
        btEqRock.Size = New Size(70, 70)
        btEqRock.TabIndex = 39
        ' 
        ' btEqJazz
        ' 
        btEqJazz.BackColor = SystemColors.Control
        btEqJazz.BorderStyle = BorderStyle.FixedSingle
        btEqJazz.Location = New Point(3, 103)
        btEqJazz.Name = "btEqJazz"
        btEqJazz.Size = New Size(70, 70)
        btEqJazz.TabIndex = 40
        ' 
        ' Label3
        ' 
        Label3.AutoSize = True
        Label3.Location = New Point(627, 14)
        Label3.Name = "Label3"
        Label3.Size = New Size(56, 13)
        Label3.TabIndex = 24
        Label3.Text = "Volumen"
        ' 
        ' GroupBox6
        ' 
        GroupBox6.Controls.Add(PanelBotones)
        GroupBox6.Location = New Point(10, 19)
        GroupBox6.Name = "GroupBox6"
        GroupBox6.Size = New Size(379, 99)
        GroupBox6.TabIndex = 33
        GroupBox6.TabStop = False
        GroupBox6.Text = "Acción: "
        ' 
        ' PanelBotones
        ' 
        PanelBotones.Controls.Add(btPausa)
        PanelBotones.Controls.Add(btAnterior)
        PanelBotones.Controls.Add(btStop)
        PanelBotones.Controls.Add(btPlay)
        PanelBotones.Controls.Add(btSiguiente)
        PanelBotones.Location = New Point(6, 15)
        PanelBotones.Name = "PanelBotones"
        PanelBotones.Size = New Size(367, 78)
        PanelBotones.TabIndex = 30
        ' 
        ' btPausa
        ' 
        btPausa.Location = New Point(151, 3)
        btPausa.Name = "btPausa"
        btPausa.Size = New Size(70, 70)
        btPausa.TabIndex = 30
        btPausa.Text = "pausa"
        btPausa.UseVisualStyleBackColor = True
        ' 
        ' btAnterior
        ' 
        btAnterior.Location = New Point(3, 3)
        btAnterior.Name = "btAnterior"
        btAnterior.Size = New Size(70, 70)
        btAnterior.TabIndex = 26
        btAnterior.Text = "anterior"
        btAnterior.UseVisualStyleBackColor = True
        ' 
        ' btStop
        ' 
        btStop.Location = New Point(297, 3)
        btStop.Name = "btStop"
        btStop.Size = New Size(70, 70)
        btStop.TabIndex = 29
        btStop.Text = "stop"
        btStop.UseVisualStyleBackColor = True
        ' 
        ' btPlay
        ' 
        btPlay.Location = New Point(78, 3)
        btPlay.Name = "btPlay"
        btPlay.Size = New Size(70, 70)
        btPlay.TabIndex = 27
        btPlay.Text = "play"
        btPlay.UseVisualStyleBackColor = True
        ' 
        ' btSiguiente
        ' 
        btSiguiente.Location = New Point(224, 3)
        btSiguiente.Name = "btSiguiente"
        btSiguiente.Size = New Size(70, 70)
        btSiguiente.TabIndex = 28
        btSiguiente.Text = "siguiente"
        btSiguiente.UseVisualStyleBackColor = True
        ' 
        ' tbVolumen
        ' 
        tbVolumen.Location = New Point(638, 49)
        tbVolumen.Name = "tbVolumen"
        tbVolumen.Orientation = Orientation.Vertical
        tbVolumen.Size = New Size(45, 160)
        tbVolumen.TabIndex = 21
        tbVolumen.TickFrequency = 2
        ' 
        ' gbMP3
        ' 
        gbMP3.Controls.Add(imgCaratula)
        gbMP3.Controls.Add(progresoMP3)
        gbMP3.Controls.Add(btExpulsarSD)
        gbMP3.Controls.Add(listaMP3)
        gbMP3.Controls.Add(LblEspacio)
        gbMP3.Controls.Add(CapacidadSD)
        gbMP3.Font = New Font("OCR A Extended", 9F, FontStyle.Regular, GraphicsUnit.Point, CByte(0))
        gbMP3.Location = New Point(707, 14)
        gbMP3.Name = "gbMP3"
        gbMP3.Size = New Size(278, 550)
        gbMP3.TabIndex = 13
        gbMP3.TabStop = False
        gbMP3.Text = "Gestión MP3: "
        ' 
        ' imgCaratula
        ' 
        imgCaratula.Location = New Point(184, 460)
        imgCaratula.Name = "imgCaratula"
        imgCaratula.Size = New Size(82, 82)
        imgCaratula.TabIndex = 16
        imgCaratula.TabStop = False
        ' 
        ' progresoMP3
        ' 
        progresoMP3.Location = New Point(17, 249)
        progresoMP3.Name = "progresoMP3"
        progresoMP3.Size = New Size(243, 23)
        progresoMP3.TabIndex = 15
        progresoMP3.Visible = False
        ' 
        ' btExpulsarSD
        ' 
        btExpulsarSD.Location = New Point(6, 499)
        btExpulsarSD.Name = "btExpulsarSD"
        btExpulsarSD.Size = New Size(93, 43)
        btExpulsarSD.TabIndex = 14
        btExpulsarSD.Text = "Expulsar microSD"
        btExpulsarSD.UseVisualStyleBackColor = True
        ' 
        ' listaMP3
        ' 
        listaMP3.FormattingEnabled = True
        listaMP3.Location = New Point(6, 63)
        listaMP3.Name = "listaMP3"
        listaMP3.Size = New Size(266, 394)
        listaMP3.TabIndex = 2
        ' 
        ' LblEspacio
        ' 
        LblEspacio.Location = New Point(17, 25)
        LblEspacio.Name = "LblEspacio"
        LblEspacio.Size = New Size(243, 19)
        LblEspacio.TabIndex = 1
        LblEspacio.Text = "---"
        LblEspacio.TextAlign = ContentAlignment.MiddleCenter
        ' 
        ' CapacidadSD
        ' 
        CapacidadSD.BackColor = SystemColors.ActiveCaption
        CapacidadSD.ForeColor = SystemColors.Control
        CapacidadSD.Location = New Point(6, 19)
        CapacidadSD.Name = "CapacidadSD"
        CapacidadSD.Size = New Size(266, 32)
        CapacidadSD.TabIndex = 0
        ' 
        ' btCerrarDB
        ' 
        btCerrarDB.Location = New Point(1721, 253)
        btCerrarDB.Name = "btCerrarDB"
        btCerrarDB.Size = New Size(92, 43)
        btCerrarDB.TabIndex = 16
        btCerrarDB.Text = "Cerrar base de datos"
        btCerrarDB.UseVisualStyleBackColor = True
        btCerrarDB.Visible = False
        ' 
        ' lblEstado
        ' 
        lblEstado.AutoSize = True
        lblEstado.Location = New Point(1266, 406)
        lblEstado.Name = "lblEstado"
        lblEstado.Size = New Size(41, 15)
        lblEstado.TabIndex = 17
        lblEstado.Text = "Label1"
        ' 
        ' lblTotal
        ' 
        lblTotal.AutoSize = True
        lblTotal.Location = New Point(1266, 431)
        lblTotal.Name = "lblTotal"
        lblTotal.Size = New Size(41, 15)
        lblTotal.TabIndex = 18
        lblTotal.Text = "Label1"
        ' 
        ' ImageList1
        ' 
        ImageList1.ColorDepth = ColorDepth.Depth32Bit
        ImageList1.ImageSize = New Size(16, 16)
        ImageList1.TransparentColor = Color.Transparent
        ' 
        ' SkControl1
        ' 
        SkControl1.Location = New Point(21, 19)
        SkControl1.Name = "SkControl1"
        SkControl1.Size = New Size(220, 220)
        SkControl1.TabIndex = 19
        SkControl1.Text = "SkControl1"
        ' 
        ' tbDensidad
        ' 
        tbDensidad.Location = New Point(6, 348)
        tbDensidad.Name = "tbDensidad"
        tbDensidad.Size = New Size(250, 45)
        tbDensidad.TabIndex = 21
        ' 
        ' cbPatron
        ' 
        cbPatron.FormattingEnabled = True
        cbPatron.Location = New Point(60, 246)
        cbPatron.Name = "cbPatron"
        cbPatron.Size = New Size(196, 21)
        cbPatron.TabIndex = 22
        ' 
        ' lblVueltas
        ' 
        lblVueltas.AutoSize = True
        lblVueltas.Location = New Point(6, 278)
        lblVueltas.Name = "lblVueltas"
        lblVueltas.Size = New Size(49, 13)
        lblVueltas.TabIndex = 23
        lblVueltas.Text = "Label1"
        ' 
        ' lblFase
        ' 
        lblFase.AutoSize = True
        lblFase.Location = New Point(6, 426)
        lblFase.Name = "lblFase"
        lblFase.Size = New Size(49, 13)
        lblFase.TabIndex = 26
        lblFase.Text = "Label1"
        ' 
        ' tbFase
        ' 
        tbFase.Location = New Point(6, 445)
        tbFase.Name = "tbFase"
        tbFase.Size = New Size(250, 45)
        tbFase.TabIndex = 25
        ' 
        ' lblDistancia
        ' 
        lblDistancia.AutoSize = True
        lblDistancia.Location = New Point(6, 375)
        lblDistancia.Name = "lblDistancia"
        lblDistancia.Size = New Size(49, 13)
        lblDistancia.TabIndex = 28
        lblDistancia.Text = "Label1"
        ' 
        ' tbDistancia
        ' 
        tbDistancia.Location = New Point(6, 394)
        tbDistancia.Name = "tbDistancia"
        tbDistancia.Size = New Size(250, 45)
        tbDistancia.TabIndex = 27
        ' 
        ' lblRotacion
        ' 
        lblRotacion.AutoSize = True
        lblRotacion.Location = New Point(6, 473)
        lblRotacion.Name = "lblRotacion"
        lblRotacion.Size = New Size(49, 13)
        lblRotacion.TabIndex = 30
        lblRotacion.Text = "Label1"
        ' 
        ' tbRotacion
        ' 
        tbRotacion.Location = New Point(6, 492)
        tbRotacion.Name = "tbRotacion"
        tbRotacion.Size = New Size(250, 45)
        tbRotacion.TabIndex = 29
        ' 
        ' gbListaPatrones
        ' 
        gbListaPatrones.Controls.Add(ckEnBucle)
        gbListaPatrones.Controls.Add(btAñadirCola)
        gbListaPatrones.Controls.Add(btParar)
        gbListaPatrones.Controls.Add(listaPatrones)
        gbListaPatrones.Controls.Add(btIniciarCola)
        gbListaPatrones.Controls.Add(btPausarCola)
        gbListaPatrones.Font = New Font("OCR A Extended", 9F)
        gbListaPatrones.Location = New Point(1259, 14)
        gbListaPatrones.Name = "gbListaPatrones"
        gbListaPatrones.Size = New Size(262, 386)
        gbListaPatrones.TabIndex = 12
        gbListaPatrones.TabStop = False
        gbListaPatrones.Text = "Cola de patrones: "
        ' 
        ' ckEnBucle
        ' 
        ckEnBucle.AutoSize = True
        ckEnBucle.Location = New Point(6, 279)
        ckEnBucle.Name = "ckEnBucle"
        ckEnBucle.Size = New Size(159, 17)
        ckEnBucle.TabIndex = 8
        ckEnBucle.Text = "Reproducir en bucle"
        ckEnBucle.UseVisualStyleBackColor = True
        ' 
        ' btAñadirCola
        ' 
        btAñadirCola.Location = New Point(6, 303)
        btAñadirCola.Name = "btAñadirCola"
        btAñadirCola.Size = New Size(249, 32)
        btAñadirCola.TabIndex = 6
        btAñadirCola.Text = "Añadir a la cola"
        btAñadirCola.UseVisualStyleBackColor = True
        ' 
        ' btParar
        ' 
        btParar.Location = New Point(175, 340)
        btParar.Name = "btParar"
        btParar.Size = New Size(79, 32)
        btParar.TabIndex = 7
        btParar.Text = "Parar"
        btParar.UseVisualStyleBackColor = True
        ' 
        ' listaPatrones
        ' 
        listaPatrones.Font = New Font("OCR A Extended", 9F, FontStyle.Regular, GraphicsUnit.Point, CByte(0))
        listaPatrones.FormattingEnabled = True
        listaPatrones.Location = New Point(6, 22)
        listaPatrones.Name = "listaPatrones"
        listaPatrones.Size = New Size(250, 238)
        listaPatrones.TabIndex = 4
        ' 
        ' btIniciarCola
        ' 
        btIniciarCola.Location = New Point(7, 341)
        btIniciarCola.Name = "btIniciarCola"
        btIniciarCola.Size = New Size(79, 32)
        btIniciarCola.TabIndex = 5
        btIniciarCola.Text = "Iniciar"
        btIniciarCola.UseVisualStyleBackColor = True
        ' 
        ' btPausarCola
        ' 
        btPausarCola.Location = New Point(91, 341)
        btPausarCola.Name = "btPausarCola"
        btPausarCola.Size = New Size(79, 32)
        btPausarCola.TabIndex = 6
        btPausarCola.Text = "Pausar"
        btPausarCola.UseVisualStyleBackColor = True
        ' 
        ' gbPatrones
        ' 
        gbPatrones.Controls.Add(lblDensidad)
        gbPatrones.Controls.Add(Label1)
        gbPatrones.Controls.Add(SkControl1)
        gbPatrones.Controls.Add(cbPatron)
        gbPatrones.Controls.Add(lblRotacion)
        gbPatrones.Controls.Add(lblVueltas)
        gbPatrones.Controls.Add(tbRotacion)
        gbPatrones.Controls.Add(tbVueltas)
        gbPatrones.Controls.Add(lblFase)
        gbPatrones.Controls.Add(lblDistancia)
        gbPatrones.Controls.Add(tbFase)
        gbPatrones.Controls.Add(tbDistancia)
        gbPatrones.Controls.Add(tbDensidad)
        gbPatrones.Font = New Font("OCR A Extended", 9F)
        gbPatrones.Location = New Point(991, 14)
        gbPatrones.Name = "gbPatrones"
        gbPatrones.Size = New Size(262, 550)
        gbPatrones.TabIndex = 13
        gbPatrones.TabStop = False
        gbPatrones.Text = "Patrones: "
        ' 
        ' lblDensidad
        ' 
        lblDensidad.AutoSize = True
        lblDensidad.Location = New Point(6, 327)
        lblDensidad.Name = "lblDensidad"
        lblDensidad.Size = New Size(49, 13)
        lblDensidad.TabIndex = 32
        lblDensidad.Text = "Label2"
        ' 
        ' Label1
        ' 
        Label1.Location = New Point(6, 249)
        Label1.Name = "Label1"
        Label1.Size = New Size(54, 15)
        Label1.TabIndex = 31
        Label1.Text = "Patrón: "
        ' 
        ' tbVueltas
        ' 
        tbVueltas.Location = New Point(6, 297)
        tbVueltas.Name = "tbVueltas"
        tbVueltas.Size = New Size(250, 45)
        tbVueltas.TabIndex = 20
        ' 
        ' gbRGB
        ' 
        gbRGB.Controls.Add(ColorPickerControl1)
        gbRGB.Controls.Add(btApagarRGB)
        gbRGB.Controls.Add(btnEnviarRGB)
        gbRGB.Controls.Add(lblBrilloRGB)
        gbRGB.Controls.Add(tbBrilloRGB)
        gbRGB.Controls.Add(lblVelocidadRGB)
        gbRGB.Controls.Add(tbVelocidadRGB)
        gbRGB.Controls.Add(Label2)
        gbRGB.Controls.Add(cbModoRGB)
        gbRGB.Location = New Point(1529, 14)
        gbRGB.Name = "gbRGB"
        gbRGB.Size = New Size(208, 550)
        gbRGB.TabIndex = 19
        gbRGB.TabStop = False
        gbRGB.Text = "Control RGB: "
        ' 
        ' ColorPickerControl1
        ' 
        ColorPickerControl1.Location = New Point(6, 70)
        ColorPickerControl1.Name = "ColorPickerControl1"
        ColorPickerControl1.Size = New Size(194, 241)
        ColorPickerControl1.TabIndex = 20
        ' 
        ' btApagarRGB
        ' 
        btApagarRGB.Location = New Point(125, 496)
        btApagarRGB.Name = "btApagarRGB"
        btApagarRGB.Size = New Size(75, 35)
        btApagarRGB.TabIndex = 42
        btApagarRGB.Text = "Apagar"
        btApagarRGB.UseVisualStyleBackColor = True
        ' 
        ' btnEnviarRGB
        ' 
        btnEnviarRGB.Location = New Point(12, 496)
        btnEnviarRGB.Name = "btnEnviarRGB"
        btnEnviarRGB.Size = New Size(75, 35)
        btnEnviarRGB.TabIndex = 41
        btnEnviarRGB.Text = "Enviar"
        btnEnviarRGB.UseVisualStyleBackColor = True
        ' 
        ' lblBrilloRGB
        ' 
        lblBrilloRGB.AutoSize = True
        lblBrilloRGB.Location = New Point(15, 370)
        lblBrilloRGB.Name = "lblBrilloRGB"
        lblBrilloRGB.Size = New Size(41, 15)
        lblBrilloRGB.TabIndex = 40
        lblBrilloRGB.Text = "Label1"
        ' 
        ' tbBrilloRGB
        ' 
        tbBrilloRGB.Location = New Point(12, 389)
        tbBrilloRGB.Name = "tbBrilloRGB"
        tbBrilloRGB.Size = New Size(188, 45)
        tbBrilloRGB.TabIndex = 39
        ' 
        ' lblVelocidadRGB
        ' 
        lblVelocidadRGB.AutoSize = True
        lblVelocidadRGB.Location = New Point(12, 319)
        lblVelocidadRGB.Name = "lblVelocidadRGB"
        lblVelocidadRGB.Size = New Size(41, 15)
        lblVelocidadRGB.TabIndex = 38
        lblVelocidadRGB.Text = "Label1"
        ' 
        ' tbVelocidadRGB
        ' 
        tbVelocidadRGB.Location = New Point(12, 338)
        tbVelocidadRGB.Name = "tbVelocidadRGB"
        tbVelocidadRGB.Size = New Size(188, 45)
        tbVelocidadRGB.TabIndex = 37
        ' 
        ' Label2
        ' 
        Label2.AutoSize = True
        Label2.Location = New Point(6, 21)
        Label2.Name = "Label2"
        Label2.Size = New Size(48, 15)
        Label2.TabIndex = 34
        Label2.Text = "Patrón: "
        ' 
        ' cbModoRGB
        ' 
        cbModoRGB.FormattingEnabled = True
        cbModoRGB.Location = New Point(6, 39)
        cbModoRGB.Name = "cbModoRGB"
        cbModoRGB.Size = New Size(194, 23)
        cbModoRGB.TabIndex = 33
        ' 
        ' timerAnteriorSiguiente
        ' 
        timerAnteriorSiguiente.Interval = 350
        ' 
        ' GroupBox9
        ' 
        GroupBox9.Controls.Add(lbCancionesMesa)
        GroupBox9.Location = New Point(412, 11)
        GroupBox9.Name = "GroupBox9"
        GroupBox9.Size = New Size(290, 324)
        GroupBox9.TabIndex = 20
        GroupBox9.TabStop = False
        GroupBox9.Text = "Lista de canciones: "
        ' 
        ' lbCancionesMesa
        ' 
        lbCancionesMesa.FormattingEnabled = True
        lbCancionesMesa.Location = New Point(6, 18)
        lbCancionesMesa.Name = "lbCancionesMesa"
        lbCancionesMesa.Size = New Size(278, 289)
        lbCancionesMesa.TabIndex = 0
        ' 
        ' Form1
        ' 
        AutoScaleDimensions = New SizeF(7F, 15F)
        AutoScaleMode = AutoScaleMode.Font
        ClientSize = New Size(1750, 570)
        Controls.Add(GroupBox9)
        Controls.Add(gbRGB)
        Controls.Add(gbPatrones)
        Controls.Add(gbListaPatrones)
        Controls.Add(lblTotal)
        Controls.Add(lblEstado)
        Controls.Add(btCerrarDB)
        Controls.Add(gbMP3)
        Controls.Add(gbControlMesa)
        Controls.Add(GroupBox2)
        Controls.Add(GroupBox1)
        Controls.Add(BtnEnviar)
        Controls.Add(TxtRecibido)
        Controls.Add(TxtEnviar)
        Controls.Add(BtnDetener)
        Controls.Add(BtnBuscar)
        Controls.Add(LabelEstado)
        FormBorderStyle = FormBorderStyle.FixedSingle
        Icon = CType(resources.GetObject("$this.Icon"), Icon)
        MaximizeBox = False
        MinimizeBox = False
        Name = "Form1"
        StartPosition = FormStartPosition.CenterScreen
        Text = "aspimaker kinetic - control de mesa cinética."
        GroupBox1.ResumeLayout(False)
        GroupBox2.ResumeLayout(False)
        gbControlMesa.ResumeLayout(False)
        gbControlMesa.PerformLayout()
        GroupBox7.ResumeLayout(False)
        panelModoRepeticion.ResumeLayout(False)
        GroupBox8.ResumeLayout(False)
        panelEqualizacion.ResumeLayout(False)
        GroupBox6.ResumeLayout(False)
        PanelBotones.ResumeLayout(False)
        CType(tbVolumen, ComponentModel.ISupportInitialize).EndInit()
        gbMP3.ResumeLayout(False)
        CType(imgCaratula, ComponentModel.ISupportInitialize).EndInit()
        CType(tbDensidad, ComponentModel.ISupportInitialize).EndInit()
        CType(tbFase, ComponentModel.ISupportInitialize).EndInit()
        CType(tbDistancia, ComponentModel.ISupportInitialize).EndInit()
        CType(tbRotacion, ComponentModel.ISupportInitialize).EndInit()
        gbListaPatrones.ResumeLayout(False)
        gbListaPatrones.PerformLayout()
        gbPatrones.ResumeLayout(False)
        gbPatrones.PerformLayout()
        CType(tbVueltas, ComponentModel.ISupportInitialize).EndInit()
        gbRGB.ResumeLayout(False)
        gbRGB.PerformLayout()
        CType(tbBrilloRGB, ComponentModel.ISupportInitialize).EndInit()
        CType(tbVelocidadRGB, ComponentModel.ISupportInitialize).EndInit()
        GroupBox9.ResumeLayout(False)
        ResumeLayout(False)
        PerformLayout()
    End Sub

    Friend WithEvents ListBoxDispositivos As ListBox
    Friend WithEvents LabelEstado As Label
    Friend WithEvents BtnBuscar As Button
    Friend WithEvents BtnDetener As Button
    Friend WithEvents ListaMesas As ListBox
    Friend WithEvents BtnConectar As Button
    Friend WithEvents TxtEnviar As TextBox
    Friend WithEvents TxtRecibido As TextBox
    Friend WithEvents BtnEnviar As Button
    Friend WithEvents GroupBox1 As GroupBox
    Friend WithEvents GroupBox2 As GroupBox
    Friend WithEvents gbControlMesa As GroupBox
    Friend WithEvents gbMP3 As GroupBox
    Friend WithEvents btExpulsarSD As Button
    Friend WithEvents CapacidadSD As ProgressBar
    Friend WithEvents LblEspacio As Label
    Friend WithEvents listaMP3 As ListBox
    Friend WithEvents progresoMP3 As ProgressBar
    Friend WithEvents btCerrarDB As Button
    Friend WithEvents lblEstado As Label
    Friend WithEvents lblTotal As Label
    Friend WithEvents ImageList1 As ImageList
    Friend WithEvents imgCaratula As PictureBox
    Friend WithEvents SkControl1 As SkiaSharp.Views.Desktop.SKControl
    Friend WithEvents tbDensidad As TrackBar
    Friend WithEvents cbPatron As ComboBox
    Friend WithEvents lblVueltas As Label
    Friend WithEvents lblFase As Label
    Friend WithEvents tbFase As TrackBar
    Friend WithEvents lblDistancia As Label
    Friend WithEvents tbDistancia As TrackBar
    Friend WithEvents lblRotacion As Label
    Friend WithEvents tbRotacion As TrackBar
    Friend WithEvents gbListaPatrones As GroupBox
    Friend WithEvents listaPatrones As ListBox
    Friend WithEvents btIniciarCola As Button
    Friend WithEvents btPausarCola As Button
    Friend WithEvents btParar As Button
    Friend WithEvents ckEnBucle As CheckBox
    Friend WithEvents gbPatrones As GroupBox
    Friend WithEvents btAñadirCola As Button
    Friend WithEvents Label1 As Label
    Friend WithEvents tbVueltas As TrackBar
    Friend WithEvents lblDensidad As Label
    Friend WithEvents gbRGB As GroupBox
    Friend WithEvents Label2 As Label
    Friend WithEvents cbModoRGB As ComboBox
    Friend WithEvents lblVelocidadRGB As Label
    Friend WithEvents tbVelocidadRGB As TrackBar
    Friend WithEvents lblBrilloRGB As Label
    Friend WithEvents tbBrilloRGB As TrackBar
    Friend WithEvents btnEnviarRGB As Button
    Friend WithEvents btApagarRGB As Button
    Friend WithEvents ColorPickerControl1 As ColorPickerControl
    Friend WithEvents tbVolumen As TrackBar
    Friend WithEvents Label3 As Label
    Friend WithEvents lbVolumen As Label
    Friend WithEvents btAnterior As Button
    Friend WithEvents btStop As Button
    Friend WithEvents btSiguiente As Button
    Friend WithEvents btPlay As Button
    Friend WithEvents PanelBotones As Panel
    Friend WithEvents btPausa As Button
    Friend WithEvents timerAnteriorSiguiente As Timer
    Friend WithEvents GroupBox6 As GroupBox
    Friend WithEvents GroupBox7 As GroupBox
    Friend WithEvents btEqNormal As BotonIconoTexto
    Friend WithEvents btEqClassic As BotonIconoTexto
    Friend WithEvents btEqJazz As BotonIconoTexto
    Friend WithEvents btEqRock As BotonIconoTexto
    Friend WithEvents btEqPop As BotonIconoTexto
    Friend WithEvents btNormal As BotonIconoTexto
    Friend WithEvents btCancionAleatoria As BotonIconoTexto
    Friend WithEvents btRepetirCancion As BotonIconoTexto
    Friend WithEvents btRepetirTodas As BotonIconoTexto
    Friend WithEvents panelModoRepeticion As Panel
    Friend WithEvents GroupBox8 As GroupBox
    Friend WithEvents panelEqualizacion As Panel
    Friend WithEvents btEqBass As BotonIconoTexto
    Friend WithEvents GroupBox9 As GroupBox
    Friend WithEvents lbCancionesMesa As ListBox

End Class
