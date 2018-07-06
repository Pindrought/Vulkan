#include "Renderer.h"
#include "FPSLimiter.h"

int main()
{

	uint32_t width = 800;
	uint32_t height = 600;
	{
		FPSLimiter fpslimiter;
		Renderer r;
		if (r.Initialize(width, height))
		{
			Timer t;
			t.Start();
			int frameCount = 0;
			while (GetAsyncKeyState(VK_F1) == false)
			{
				if (!r.ProcessMessages())
					break;
				if (!r.DrawFrame())
					break;
				fpslimiter.Pulse(1000);
				frameCount += 1;
				double elapseTime = t.GetMilisecondsElapsed();
				if (elapseTime >= 1000)
				{
					t.Restart();
					std::cout << "FPS: " << frameCount << std::endl;;
					frameCount = 0;
				}
			}
		}
		else
		{
			std::cerr << "Failed to initialize render." << std::endl;
		}
	}
	system("pause");
	return 0;
}