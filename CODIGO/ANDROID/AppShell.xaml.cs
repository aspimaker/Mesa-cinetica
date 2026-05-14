using app_mesa_cinetica.Views;
using CommunityToolkit.Maui.Alerts;
using CommunityToolkit.Maui.Core;
using Font = Microsoft.Maui.Font;

namespace app_mesa_cinetica
{
    public partial class AppShell : Shell
    {
        public AppShell()
        {
           InitializeComponent();
           Routing.RegisterRoute("bleconnect", typeof(BleConnectPage));
        }
      
    }
}
