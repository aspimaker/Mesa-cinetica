using app_mesa_cinetica.Models;
using CommunityToolkit.Mvvm.Input;

namespace app_mesa_cinetica.PageModels
{
    public interface IProjectTaskPageModel
    {
        IAsyncRelayCommand<ProjectTask> NavigateToTaskCommand { get; }
        bool IsBusy { get; }
    }
}