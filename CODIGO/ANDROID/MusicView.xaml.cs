using Microsoft.Maui.Controls;

namespace app_mesa_cinetica.Views;

public partial class MusicView : ContentView
{
    private record TrackInfo(string Title, string Meta, string Duration);

    private readonly List<TrackInfo> _tracks = new()
    {
        new("Desert Drift",    "Track 1 · Electrónica", "4:02"),
        new("Zen Garden Loop", "Track 2 · Ambiental",   "3:24"),
        new("Orbital Sands",   "Track 3 · Ambiental",   "5:17"),
        new("Crystal Spiral",  "Track 4 · Clásica",     "2:48"),
        new("Wave Form",       "Track 5 · Electrónica", "6:11"),
    };

    private int _currentTrack = 1;
    private bool _isPlaying = false;
    private bool _isLoop = false;

    public MusicView() => InitializeComponent();

    private void OnTrackTapped(object? sender, TappedEventArgs e)
    {
        if (e.Parameter is string p && int.TryParse(p, out int idx)) SelectTrack(idx);
    }

    private void SelectTrack(int idx)
    {
        _currentTrack = idx;
        var t = _tracks[idx];
        LblTrackTitle.Text = t.Title; LblTrackMeta.Text = t.Meta;
        LblTimeTotal.Text = t.Duration; LblTimeCurrent.Text = "0:00";
        SliderProgress.Value = 0; _isPlaying = true; BtnPlay.Source = "icon_pause.png";
    }

    private void OnTogglePlay(object? sender, EventArgs e)
    {
        _isPlaying = !_isPlaying;
        BtnPlay.Source = _isPlaying ? "icon_pause.png" : "icon_play.png";
    }

    private void OnStopTrack(object? sender, EventArgs e)
    {
        _isPlaying = false; BtnPlay.Source = "icon_play.png";
        SliderProgress.Value = 0; LblTimeCurrent.Text = "0:00";
    }

    private void OnToggleLoop(object? sender, EventArgs e)
    {
        _isLoop = !_isLoop;
        BtnLoop.Opacity = _isLoop ? 1.0 : 0.4;
    }

    private void OnPrevTrack(object? sender, EventArgs e)
        => SelectTrack((_currentTrack - 1 + _tracks.Count) % _tracks.Count);

    private void OnNextTrack(object? sender, EventArgs e)
        => SelectTrack((_currentTrack + 1) % _tracks.Count);

    private void OnSeek(object? sender, EventArgs e) { }
}