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
        private AudioDecoder decoder = new AudioDecoder();
        private Dictionary<string, IBuffer> buffers = new Dictionary<string, IBuffer>();

        public MainPage()
        {
            this.InitializeComponent();
            this.NavigationCacheMode = NavigationCacheMode.Required;
        }

        private async void OnToggleBeatClicked(object sender, RoutedEventArgs e)
        {
            var button = sender as ToggleButton;

            await this.ToggleSample("beat", button.IsChecked.Value);
        }

        private async void OnToggleMelodyClicked(object sender, RoutedEventArgs e)
        {
            var button = sender as ToggleButton;

            await this.ToggleSample("pad", button.IsChecked.Value);
        }

        private async Task ToggleSample(string name, bool isPlaying)
        {
            var buffer = await this.GetBuffer(name);
            
            var sample = new AudioSample(name, buffer);

            if (isPlaying)
            {
                this.player.Play(sample);
            }
            else
            {
                this.player.Stop(sample);
            }
        }

        private async Task<IBuffer> GetBuffer(string sampleName)
        {
            if (!this.buffers.ContainsKey(sampleName))
            {
                var path = String.Format("ms-appx:///Assets/{0}.mp3", sampleName);
                var audioFile = await StorageFile.GetFileFromApplicationUriAsync(new Uri(path));
                var audioFileStream = await audioFile.OpenReadAsync();
                var wavBytes = this.decoder.Decode(audioFileStream);
                var buffer = wavBytes.AsBuffer();
                this.buffers[sampleName] = buffer;
            }

            return this.buffers[sampleName];
        }
    }
}
