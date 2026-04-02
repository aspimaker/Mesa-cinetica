using app_mesa_cinetica.Models;
using app_mesa_cinetica.PageModels;

namespace app_mesa_cinetica.Pages
{
    public partial class MainPage : ContentPage
    {
        public MainPage(MainPageModel model)
        {
            InitializeComponent();
            BindingContext = model;
        }
    }
}