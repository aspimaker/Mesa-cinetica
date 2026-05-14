using Microsoft.Maui.Controls;
using Microsoft.Maui.Graphics;
using Microsoft.Maui.Controls.Shapes;

namespace app_mesa_cinetica.Views;

// ── Modelo de trazos ────────────────────────────────────────────────────────

public enum DrawTool { Pen, Line, Circle }

public class DrawStroke
{
    public DrawTool Tool { get; set; } = DrawTool.Pen;
    public List<PointF> Points { get; } = new();
    public Color Color { get; set; } = Colors.Black;
    public float Size { get; set; } = 3f;
}

// ── IDrawable ───────────────────────────────────────────────────────────────

public class CanvasDrawable : IDrawable
{
    public List<DrawStroke> Strokes { get; } = new();
    public DrawStroke? Current { get; set; }

    public void Draw(ICanvas canvas, RectF dirty)
    {
        canvas.FillColor = Color.FromArgb("#E8D5B0");  //Colors.White;
        canvas.FillRectangle(dirty);
        foreach (var s in Strokes) Render(canvas, s);
        if (Current != null) Render(canvas, Current);
    }

    private static void Render(ICanvas canvas, DrawStroke s)
    {
        canvas.StrokeColor = s.Color;
        canvas.StrokeSize = s.Size;
        canvas.StrokeLineCap = LineCap.Round;
        canvas.StrokeLineJoin = LineJoin.Round;

        switch (s.Tool)
        {
            case DrawTool.Pen when s.Points.Count > 1:
                var path = new PathF();
                path.MoveTo(s.Points[0]);
                for (int i = 1; i < s.Points.Count; i++) path.LineTo(s.Points[i]);
                canvas.DrawPath(path);
                break;

            case DrawTool.Line when s.Points.Count >= 2:
                canvas.DrawLine(s.Points[0], s.Points[^1]);
                break;

            case DrawTool.Circle when s.Points.Count >= 2:
                var p0 = s.Points[0]; var p1 = s.Points[^1];
                float r = MathF.Sqrt(MathF.Pow(p1.X - p0.X, 2) + MathF.Pow(p1.Y - p0.Y, 2));
                canvas.DrawCircle(p0.X, p0.Y, r);
                break;
        }
    }
}

// ── ContentView ─────────────────────────────────────────────────────────────

public partial class DrawView : ContentView
{
    private readonly CanvasDrawable _drawable = new();
    private DrawTool _tool = DrawTool.Pen;
    private float _size = 3f;
    private Color _color = Colors.Black;

    private Dictionary<DrawTool, Border> _toolBtns = null!;

    public DrawView()
    {
        InitializeComponent();
        DrawingCanvas.Drawable = _drawable;
        DrawingCanvas.SizeChanged += OnCanvasSizeChanged;
        _toolBtns = new()
        {
            [DrawTool.Pen] = BtnPen,
            [DrawTool.Line] = BtnLine,
            [DrawTool.Circle] = BtnCircle,
        };
    }

    // ── Herramientas ─────────────────────────────────────────────────────────

    private void OnCanvasSizeChanged(object? sender, EventArgs e)
    {
        double size = DrawingCanvas.Width;
        DrawingCanvas.HeightRequest = size;
        DrawingCanvas.Clip = new EllipseGeometry
        {
            Center = new Point(size / 2, size / 2),
            RadiusX = size / 2,
            RadiusY = size / 2
        };
    }

    private void OnToolSelected(object? sender, TappedEventArgs e)
    {
        if (e.Parameter is not string t) return;
        _tool = t switch { "line" => DrawTool.Line, "circle" => DrawTool.Circle, _ => DrawTool.Pen };

        bool dark = Application.Current!.RequestedTheme == AppTheme.Dark;
        Color activeBg = dark ? Color.FromArgb("#3A3A3C") : Color.FromArgb("#F2F2F7");
        Color activeStr = dark ? Color.FromArgb("#C7C7CC") : Color.FromArgb("#C7C7CC");
        Color neutral = dark ? Color.FromArgb("#3A3A3C") : Color.FromArgb("#E5E5EA");

        foreach (var (tool, btn) in _toolBtns)
        {
            bool active = tool == _tool;
            btn.BackgroundColor = active ? activeBg : Colors.Transparent;
            btn.Stroke = active ? activeStr : neutral;
            if (btn.Content is Label lbl)
                lbl.TextColor = active
                    ? (dark ? Colors.White : Colors.Black)
                    : Color.FromArgb("#8E8E93");
        }
    }

    // ── Eventos de dibujo ────────────────────────────────────────────────────

    private void OnDrawStart(object? sender, TouchEventArgs e)
    {
        _drawable.Current = new DrawStroke { Tool = _tool, Color = _color, Size = _size };
        _drawable.Current.Points.Add(e.Touches[0]);
        DrawingCanvas.Invalidate();
    }

    private void OnDrawDrag(object? sender, TouchEventArgs e)
    {
        if (_drawable.Current == null) return;
        var pt = e.Touches[0];
        if (_tool == DrawTool.Pen)
            _drawable.Current.Points.Add(pt);
        else
        {
            if (_drawable.Current.Points.Count > 1)
                _drawable.Current.Points.RemoveAt(_drawable.Current.Points.Count - 1);
            _drawable.Current.Points.Add(pt);
        }
        DrawingCanvas.Invalidate();
    }

    private void OnDrawEnd(object? sender, TouchEventArgs e)
    {
        if (_drawable.Current == null) return;
        _drawable.Strokes.Add(_drawable.Current);
        _drawable.Current = null;
        DrawingCanvas.Invalidate();
    }

    // ── Grosor ───────────────────────────────────────────────────────────────

    private void OnSizeChanged(object? sender, ValueChangedEventArgs e)
    {
        _size = (float)e.NewValue;
        LblSizeVal.Text = ((int)_size).ToString();
        float px = Math.Max(6f, _size * 2f);
        SizePreview.WidthRequest = px;
        SizePreview.HeightRequest = px;
    }

    // ── Acciones ─────────────────────────────────────────────────────────────

    private void OnClearCanvas(object? sender, TappedEventArgs e)
    {
        _drawable.Strokes.Clear();
        _drawable.Current = null;
        DrawingCanvas.Invalidate();
    }

    private void OnSendToTable(object? sender, EventArgs e)
    {
        // TODO: serializar trazos y enviar al servicio de la mesa cinética
    }

    private void OnSaveCanvas(object? sender, EventArgs e)
    {
        // TODO: exportar GraphicsView a imagen
    }
}