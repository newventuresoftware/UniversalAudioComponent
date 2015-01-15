using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Runtime.InteropServices.WindowsRuntime;
using System.Threading.Tasks;
using Windows.Foundation;
using Windows.Foundation.Collections;
using Windows.Storage;
using Windows.Storage.Streams;
using Windows.UI.Xaml;
using Windows.UI.Xaml.Controls;
using Windows.UI.Xaml.Controls.Primitives;
using Windows.UI.Xaml.Data;
using Windows.UI.Xaml.Input;
using Windows.UI.Xaml.Media;
using Windows.UI.Xaml.Navigation;

// The Blank Page item template is documented at http://go.microsoft.com/fwlink/?LinkId=234238

namespace UniversalAudioComponent.App
{
    /// <summary>
    /// An empty page that can be used on its own or navigated to within a Frame.
    /// </summary>
    public sealed partial class MainPage : Page
    {
        private UniversalAudioPlayer player = new UniversalAudioPlayer();
        private Dictionary<string, IBuffer> buffers = new Dictionary<string, IBuffer>();
        
        public MainPage()
        {
            this.InitializeComponent();
            this.NavigationCacheMode = NavigationCacheMode.Required;
        }

        protected override void OnNavigatedTo(NavigationEventArgs e)
        {
        }

        private async Task<IBuffer> GetBuffer(string sampleName)
        {
            IBuffer buffer = null;

            if(!this.buffers.ContainsKey(sampleName))
            {
                var path = String.Format("ms-appx:///Assets/{0}.wav", sampleName);
                var audioFile = await StorageFile.GetFileFromApplicationUriAsync(new Uri(path));
                buffer = await FileIO.ReadBufferAsync(audioFile);

                this.buffers[sampleName] = buffer;
            }
            else
            {
                buffer = this.buffers[sampleName];
            }

            return buffer;
        }

        private async void OnToggleBeatClicked(object sender, RoutedEventArgs e)
        {
            var button = sender as ToggleButton;
            var buffer = await this.GetBuffer("beat");
            var sample = new AudioSample("beat", buffer);

            if(button.IsChecked.Value)
            {
                this.player.Play(sample);
            }
            else
            {
                this.player.Stop(sample);
            }
        }

        private async void OnToggleMelodyClicked(object sender, RoutedEventArgs e)
        {
            var button = sender as ToggleButton;
            var buffer = await this.GetBuffer("pad");
            var sample = new AudioSample("pad", buffer);

            if (button.IsChecked.Value)
            {
                this.player.Play(sample);
            }
            else
            {
                this.player.Stop(sample);
            }
        }
    }
}
