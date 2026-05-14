namespace app_mesa_cinetica;

public partial class SplashPage : ContentPage
{
    public SplashPage()
    {
        InitializeComponent();
    }

    protected override async void OnAppearing()
    {
        base.OnAppearing();

        await LogoImage.FadeToAsync(1, 800);// Fade in del logo
        await Task.Delay(3000); // 3 segundos
        await LogoImage.FadeToAsync(0, 800); // Fade out

        // Pequeño delay extra antes de cambiar de página
        await Task.Delay(100);

        MainThread.BeginInvokeOnMainThread(() =>
        {
            var appShell = Handler?.MauiContext?.Services?.GetService<AppShell>();
            if (appShell != null && Application.Current != null)
                Application.Current.Windows[0].Page = appShell;
        });

        //Application.Current.MainPage = new NavigationPage(new MainPage());

    }
}