using Android.App;
using Android.Content.PM;
using Android.OS;

namespace app_mesa_cinetica
{
    [Activity(Theme = "@style/Maui.SplashTheme", MainLauncher = true, LaunchMode = LaunchMode.SingleTop, ConfigurationChanges = ConfigChanges.ScreenSize | ConfigChanges.Orientation | ConfigChanges.UiMode | ConfigChanges.ScreenLayout | ConfigChanges.SmallestScreenSize | ConfigChanges.Density)]
    public class MainActivity : MauiAppCompatActivity
    {

        protected override void OnCreate(Bundle? savedInstanceState)
        {
            base.OnCreate(savedInstanceState);

            // Mejorar rendimiento del canvas en Android
            Microsoft.Maui.Handlers.GraphicsViewHandler.Mapper.AppendToMapping("MyCustomization", (handler, view) =>
            {
                if (handler.PlatformView is Android.Views.View nativeView)
                {
                    nativeView.SetLayerType(Android.Views.LayerType.Hardware, null);
                }
            });
        }

    }
}
