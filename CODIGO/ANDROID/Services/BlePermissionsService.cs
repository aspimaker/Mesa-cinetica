namespace app_mesa_cinetica.Services;

public class BlePermissionsService
{
    public async Task<bool> RequestPermissionsAsync()
    {
        if (DeviceInfo.Platform == DevicePlatform.Android)
        {
            var location = await Permissions.RequestAsync<Permissions.LocationWhenInUse>();
            if (location != PermissionStatus.Granted)
                return false;

            // Android 12+ necesita permisos explícitos de BT
            if (DeviceInfo.Version.Major >= 12)
            {
                var btConnect = await Permissions.RequestAsync<Permissions.Bluetooth>();
                if (btConnect != PermissionStatus.Granted)
                    return false;
            }
        }
        return true;
    }
}