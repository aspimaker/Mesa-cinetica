using Plugin.BLE;
using Plugin.BLE.Abstractions.Contracts;
using Plugin.BLE.Abstractions.EventArgs;
using System.Text;

namespace app_mesa_cinetica.Services;

public class BleService
{
    private readonly IBluetoothLE _ble;
    private readonly IAdapter _adapter;
    private IDevice? _connectedDevice;

    // UUID del servicio y característica
    private static readonly Guid ServiceUuid =         Guid.Parse("0000FFE0-0000-1000-8000-00805F9B34FB");
    private static readonly Guid CharacteristicUuid =  Guid.Parse("0000FFE1-0000-1000-8000-00805F9B34FB");

    public List<IDevice> DiscoveredDevices { get; } = new();

    public event Action<IDevice>? DeviceFound;
    public event Action<string>? StatusChanged;

    public BleService()
    {
        _ble = CrossBluetoothLE.Current;
        _adapter = CrossBluetoothLE.Current.Adapter;
        _adapter.ScanTimeout = 10000; // 10 segundos

        _adapter.DeviceDiscovered += OnDeviceDiscovered;
        _adapter.DeviceConnected += (s, e) => StatusChanged?.Invoke("Conectado");
        _adapter.DeviceDisconnected += (s, e) => StatusChanged?.Invoke("Desconectado");
    }


    public static string FormatearFechaHora(DateTime fecha)
    {
        int diaNumero = fecha.DayOfWeek == DayOfWeek.Sunday ? 7 : (int)fecha.DayOfWeek;
        return fecha.ToString("ddMMyyHHmmss") + diaNumero.ToString();
    }

    public async Task EnviarFechaHoraAsync()
    {
        var fechaHora = FormatearFechaHora(DateTime.Now);
        var mensaje = Constants.InicioMensaje + "SINCROFECHA" + fechaHora + Constants.FinMensaje;
        var data = Encoding.UTF8.GetBytes(mensaje);

        await WriteAsync(ServiceUuid, CharacteristicUuid, data);
    }



    // ── ESCANEAR ──────────────────────────────────────────────────────


    public async Task StartScanAsync()
    {
        if (_ble.State != BluetoothState.On)
        {
            StatusChanged?.Invoke("Bluetooth desactivado");
            return;
        }

        DiscoveredDevices.Clear();
        StatusChanged?.Invoke("Escaneando...");
        await _adapter.StartScanningForDevicesAsync();
    }

    public async Task StopScanAsync()
        => await _adapter.StopScanningForDevicesAsync();

    private void OnDeviceDiscovered(object? sender, DeviceEventArgs e)
    {
        if (!DiscoveredDevices.Contains(e.Device))
        {
            DiscoveredDevices.Add(e.Device);
            DeviceFound?.Invoke(e.Device);
        }
    }

    // ── CONECTAR ──────────────────────────────────────────────────────

    public async Task<bool> ConnectAsync(IDevice device)
    {
        try
        {
            await _adapter.ConnectToDeviceAsync(device);
            _connectedDevice = device;
            return true;
        }
        catch (Exception ex)
        {
            StatusChanged?.Invoke($"Error al conectar: {ex.Message}");
            return false;
        }
    }

    public async Task DisconnectAsync()
    {
        if (_connectedDevice is not null)
            await _adapter.DisconnectDeviceAsync(_connectedDevice);
    }

    public bool IsConnected => _connectedDevice?.State == Plugin.BLE.Abstractions.DeviceState.Connected;

    // ── LEER ──────────────────────────────────────────────────────────

    public async Task<byte[]?> ReadAsync(Guid serviceUuid, Guid characteristicUuid)
    {
        if (_connectedDevice is null) return null;

        var service = await _connectedDevice.GetServiceAsync(serviceUuid);
        if (service is null) return null;

        var characteristic = await service.GetCharacteristicAsync(characteristicUuid);
        if (characteristic is null) return null;

        var result = await characteristic.ReadAsync();
        return result.data;
    }

    // ── ESCRIBIR ──────────────────────────────────────────────────────

    public async Task<bool> WriteAsync(Guid serviceUuid, Guid characteristicUuid, byte[] data)
    {
        if (_connectedDevice is null) return false;

        var service = await _connectedDevice.GetServiceAsync(serviceUuid);
        if (service is null) return false;

        var characteristic = await service.GetCharacteristicAsync(characteristicUuid);
        if (characteristic is null) return false;

        await characteristic.WriteAsync(data);
        return true;
    }

    // ── NOTIFICACIONES ────────────────────────────────────────────────

    public async Task<bool> SubscribeAsync(Guid serviceUuid, Guid characteristicUuid,
                                            Action<byte[]> onDataReceived)
    {
        if (_connectedDevice is null) return false;

        var service = await _connectedDevice.GetServiceAsync(serviceUuid);
        if (service is null) return false;

        var characteristic = await service.GetCharacteristicAsync(characteristicUuid);
        if (characteristic is null) return false;

        characteristic.ValueUpdated += (s, e) =>
        {
            if (e.Characteristic.Value is not null)
                onDataReceived.Invoke(e.Characteristic.Value);
        };

        await characteristic.StartUpdatesAsync();
        return true;
    }

    public async Task UnsubscribeAsync(Guid serviceUuid, Guid characteristicUuid)
    {
        if (_connectedDevice is null) return;

        var service = await _connectedDevice.GetServiceAsync(serviceUuid);
        if (service is null) return;

        var characteristic = await service.GetCharacteristicAsync(characteristicUuid);
        if (characteristic is null) return;

        await characteristic.StopUpdatesAsync();
    }
}