#ifndef Camera_H
#define Camera_H

class Camera
{
public:
	void updateCameraPosition(float velocityX, float velocityY, int playerX, int playerY);
	void setDisplay(int displayW, int displayH);
	void setCameraMaxBounds(int x, int y);

	int getXPos();
	int getYPos();
	int getUpperBoundWidth(int tileSize);
	int getUpperBoundHeight(int tileSize);
	int getLowerBoundWidth(int tileSize);
	int getLowerBoundHeight(int tileSize);

	bool pastCameraBoundaryX(int x);
	bool pastCameraBoundaryY(int y);

private:
	int x = 0, y = 0;
	int displayWidth = 0, displayHeight = 0, mapX, mapY;
};

#endif