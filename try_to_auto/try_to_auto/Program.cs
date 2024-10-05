using System;
using System.Drawing;
using System.IO;
using System.Runtime.InteropServices;
using System.Threading;
using System.Windows.Forms;
using Emgu.CV;
using Emgu.CV.CvEnum;
using Emgu.CV.Structure;
using Emgu.CV.Util;

namespace try_to_auto
{
    internal class Program
    {
        [DllImport("user32.dll")]
        private static extern bool SetCursorPos(int x, int y);

        [DllImport("user32.dll")]
        private static extern void mouse_event(int dwFlags, int dx, int dy, int cButtons, int dwExtraInfo);

        private const int MOUSEEVENTF_LEFTDOWN = 0x02;
        private const int MOUSEEVENTF_LEFTUP = 0x04;

        static void Click(int x, int y)
        {
            SetCursorPos(x, y);
            mouse_event(MOUSEEVENTF_LEFTDOWN, x, y, 0, 0);
            Thread.Sleep(50);  // Задержка
            mouse_event(MOUSEEVENTF_LEFTUP, x, y, 0, 0);
        }

        static Mat CaptureScreen()
        {
            // Определяем область захвата экрана
            Rectangle bounds = Screen.PrimaryScreen.Bounds;
            Bitmap screenshot = new Bitmap(bounds.Width, bounds.Height);

            using (Graphics g = Graphics.FromImage(screenshot))
            {
                g.CopyFromScreen(Point.Empty, Point.Empty, bounds.Size);
            }

            return BitmapToMat(screenshot); // Преобразуем Bitmap в Mat через вспомогательную функцию
        }
        static Mat BitmapToMat(Bitmap bitmap)
        {
            Mat mat = new Mat();
            using (MemoryStream ms = new MemoryStream())
            {
                bitmap.Save(ms, System.Drawing.Imaging.ImageFormat.Bmp);
                mat = CvInvoke.Imdecode(ms.ToArray(), ImreadModes.Color);
            }
            return mat;
        }

        static void FindAndClickSnowflakes()
        {
            while (true)
            {
                using (Mat screenMat = CaptureScreen())
                {
                    Mat hsvImage = new Mat();
                    CvInvoke.CvtColor(screenMat, hsvImage, ColorConversion.Bgr2Hsv);

                    // Устанавливаем диапазон цвета снежинок
                    var lowerColor = new ScalarArray(new MCvScalar(90, 0, 200)); // Замените на нужный диапазон
                    var upperColor = new ScalarArray(new MCvScalar(110, 50, 255));

                    Mat mask = new Mat();
                    CvInvoke.InRange(hsvImage, lowerColor, upperColor, mask);

                    // Поиск контуров
                    var contours = new VectorOfVectorOfPoint();
                    CvInvoke.FindContours(mask, contours, null, RetrType.External, ChainApproxMethod.ChainApproxSimple);

                    for (int i = 0; i < contours.Size; i++)
                    {
                        var rect = CvInvoke.BoundingRectangle(contours[i]);
                        int centerX = rect.X + rect.Width / 2;
                        int centerY = rect.Y + rect.Height / 2;

                        // Перемещение мыши к центру снежинки и клик
                        Click(centerX, centerY);
                        Thread.Sleep(new Random().Next(100, 300));  // Случайная задержка
                    }
                }
                Thread.Sleep(100); // Задержка перед следующим захватом экрана
            }
        }

        static void Main(string[] args)
        {
            FindAndClickSnowflakes();
        }
    }
}
