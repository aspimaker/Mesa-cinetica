using Microsoft.Maui.Controls;

namespace app_mesa_cinetica.Views;

public partial class PatternsView : ContentView
{
    private static Color Accent => Color.FromArgb("#007AFF");
    private static Color Neutral => Application.Current!.RequestedTheme == AppTheme.Dark
                                      ? Color.FromArgb("#3A3A3C") : Color.FromArgb("#E5E5EA");
    private static Color ActiveBg => Application.Current!.RequestedTheme == AppTheme.Dark
                                      ? Color.FromArgb("#3A3A3C") : Color.FromArgb("#F2F2F7");

    private string _selectedPattern = "Espiral 3×";
    private string _selectedFinish = "stop";
    private Dictionary<string, Border> _patternCards = null!;
    private Dictionary<string, Border> _finishCards = null!;

    public PatternsView()
    {
        InitializeComponent();
        _patternCards = new()
        {
            ["Espiral 3×"] = CardEspiral,
            ["Roseta"] = CardRoseta,
            ["Zig-zag"] = CardZigzag,
            ["Lissajous"] = CardLissajous,
            ["Mandala"] = CardMandala,
            ["Personalizado"] = CardPersonalizado,
        };
        _finishCards = new()
        {
            ["stop"] = FinishStop,
            ["repeat"] = FinishRepeat,
            ["next"] = FinishNext,
        };
    }

    private void OnPatternSelected(object? sender, TappedEventArgs e)
    {
        if (e.Parameter is not string name) return;
        _selectedPattern = name;
        foreach (var (k, c) in _patternCards)
        {
            c.Stroke = k == name ? Accent : Neutral;
            c.StrokeThickness = k == name ? 1.5 : 0.5;
        }
    }

    private void OnFinishSelected(object? sender, TappedEventArgs e)
    {
        if (e.Parameter is not string opt) return;
        _selectedFinish = opt;
        foreach (var (k, c) in _finishCards)
        {
            c.BackgroundColor = k == opt ? ActiveBg : Colors.Transparent;
            c.Stroke = k == opt ? (Application.Current!.RequestedTheme == AppTheme.Dark
                       ? Color.FromArgb("#48484A") : Color.FromArgb("#C7C7CC")) : Neutral;
        }
    }

    private void OnAddToQueue(object? sender, TappedEventArgs e) { /* TODO */ }
    private void OnRunPattern(object? sender, EventArgs e) { /* TODO */ }
}