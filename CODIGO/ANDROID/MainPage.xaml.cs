using app_mesa_cinetica.Services;
using app_mesa_cinetica.Views;
using Microsoft.Maui.Controls;
using Syncfusion.Maui.Toolkit.SegmentedControl;

namespace app_mesa_cinetica;

public partial class MainPage : ContentPage
{
    private MusicView? _musicView;
    private PatternsView? _patternsView;
    private DrawView? _drawView;

    private enum Tab { Music, Patterns, Draw }
    private Tab _activeTab = Tab.Music;

    private static Color ActiveBg => Application.Current!.RequestedTheme == AppTheme.Dark
                                        ? Color.FromArgb("#3A3A3C") : Color.FromArgb("#F2F2F7");
    private static Color ActiveText => Application.Current!.RequestedTheme == AppTheme.Dark
                                        ? Color.FromArgb("#FFFFFF") : Color.FromArgb("#000000");
    private static Color InactiveText => Color.FromArgb("#8E8E93");

    public MainPage()
    {
        InitializeComponent();
        ShowTab(Tab.Music);
    }

    protected override async void OnAppearing()
    {
        base.OnAppearing();

        var permissions = new BlePermissionsService();
        var granted = await permissions.RequestPermissionsAsync();

        if (!granted)
        {
            await DisplayAlertAsync("Permisos",
                "Se necesitan permisos de Bluetooth y ubicación", "OK");
        }
    }

    private async void OnMenuTapped(object? sender, TappedEventArgs e)
    {
        var opcion = await DisplayActionSheetAsync(
            "Menú",
            "Cancelar",
            null,
            "🔵  Conectar mesa cinética",
            "⚙️  Ajustes",
            "ℹ️  Acerca de");

        switch (opcion)
        {
            case "🔵  Conectar mesa cinética":
                await Shell.Current.GoToAsync("bleconnect");
                break;
            case "⚙️  Ajustes":
                // await Navigation.PushAsync(new SettingsPage());
                break;
            case "ℹ️  Acerca de":
                await DisplayAlertAsync("Acerca de", "Control de Mesa Cinética v 1.0- (c) aspimaker 2026", "OK");
                break;
        }
    }

    // ── Tabs ─────────────────────────────────────────────────────────

    private void OnTabMusic(object? sender, TappedEventArgs e) => ShowTab(Tab.Music);
    private void OnTabPatterns(object? sender, TappedEventArgs e) => ShowTab(Tab.Patterns);
    private void OnTabDraw(object? sender, TappedEventArgs e) => ShowTab(Tab.Draw);

    private void ShowTab(Tab tab)
    {
        if (_activeTab == tab && MainContent.Content != null) return;
        _activeTab = tab;

        MainContent.Content = tab switch
        {
            Tab.Music => _musicView ??= new MusicView(),
            Tab.Patterns => _patternsView ??= new PatternsView(),
            Tab.Draw => _drawView ??= new DrawView(),
            _ => _musicView ??= new MusicView(),
        };

        UpdateTabStyle(TabMusic, GetTabLabel(TabMusic), tab == Tab.Music);
        UpdateTabStyle(TabPatterns, GetTabLabel(TabPatterns), tab == Tab.Patterns);
        UpdateTabStyle(TabDraw, GetTabLabel(TabDraw), tab == Tab.Draw);
    }

    private static void UpdateTabStyle(Border tab, Label? label, bool active)
    {
        tab.BackgroundColor = active ? ActiveBg : Colors.Transparent;
        if (label is null) return;
        label.TextColor = active ? ActiveText : InactiveText;
        label.FontAttributes = active ? FontAttributes.Bold : FontAttributes.None;
    }

    private static Label? GetTabLabel(Border tab)
    {
        if (tab.Content is VerticalStackLayout vsl && vsl.Count > 1)
            return vsl[1] as Label;
        return null;
    }

    // ── Tema ─────────────────────────────────────────────────────────
    private void SfSegmentedControl_SelectionChanged(object? sender, EventArgs e)
    {
        Application.Current!.UserAppTheme =
            ThemeSegmentedControl.SelectedIndex == 1 ? AppTheme.Dark : AppTheme.Light;
    }
}