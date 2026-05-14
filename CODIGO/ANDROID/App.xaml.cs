using Microsoft.Extensions.DependencyInjection;

namespace app_mesa_cinetica
{
    public partial class App : Application
    {
        public App()
        {
            InitializeComponent();
        }

        protected override Window CreateWindow(IActivationState? activationState)
        {
            return new Window(new SplashPage()); // Arranca en el splash
            //return new Window(new AppShell());
        }
    }
}