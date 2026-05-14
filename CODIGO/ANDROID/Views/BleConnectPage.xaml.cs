using app_mesa_cinetica.Services;
using Plugin.BLE.Abstractions.Contracts;

namespace app_mesa_cinetica.Views;

public partial class BleConnectPage : ContentPage
{
    private readonly BleService _bleService = new();
    private readonly List<IDevice> _filteredDevices = new();

    public BleConnectPage()
    {
        InitializeComponent();

        _bleService.StatusChanged += status =>
            MainThread.BeginInvokeOnMainThread(() =>
                StatusLabel.Text = status);

        _bleService.DeviceFound += OnDeviceFound;
    }

    protected override async void OnAppearing()
    {
        base.OnAppearing();
        await IniciarBusquedaAsync();
    }

    private void OnDeviceFound(IDevice device)
    {
        // Filtrar solo dispositivos que empiecen por "aspiKntc"
        if (device.Name?.StartsWith("aspiKntc") != true) return;

        MainThread.BeginInvokeOnMainThread(() =>
        {
            _filteredDevices.Add(device);
            DeviceList.ItemsSource = null;
            DeviceList.ItemsSource = _filteredDevices;
        });
    }

    private async void OnScanClicked(object? sender, EventArgs e)
    {
        await IniciarBusquedaAsync(); 
    }

    private async Task IniciarBusquedaAsync()
    {
        _filteredDevices.Clear();
        DeviceList.ItemsSource = null;
        ScanButton.IsEnabled = false;
        ScanButton.Text = "Buscando...";

        await _bleService.StartScanAsync();

        ScanButton.IsEnabled = true;
        ScanButton.Text = "Buscar de nuevo";

        if (_filteredDevices.Count == 0)
            StatusLabel.Text = "No se encontraron dispositivos";
    }

    private async void OnDeviceSelected(object? sender, SelectionChangedEventArgs e)
    {
        if (e.CurrentSelection.FirstOrDefault() is not IDevice device) return;

        StatusLabel.Text = $"Conectando a {device.Name}...";
        var ok = await _bleService.ConnectAsync(device);

        if (ok)
        {
            await _bleService.EnviarFechaHoraAsync();
            System.Diagnostics.Debug.WriteLine("[BLE] EnviarFechaHoraAsync completado");

            await DisplayAlertAsync("Conectado", $"Conectado a {device.Name}", "OK");
            await Shell.Current.GoToAsync("..");
        }
        else
        {
            StatusLabel.Text = "Error al conectar";
            if (sender is CollectionView cv)
                cv.SelectedItem = null!;
        }
    }


}