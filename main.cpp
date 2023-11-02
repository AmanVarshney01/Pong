#include "raylib.h"

struct Ball {
	float x, y;
	float speedX, speedY;
	float radius;

	void Draw() {
		DrawCircle(x, y, radius, WHITE);
	}
};

struct Paddle {
	float x, y;
	float speed;
	float width, height;

	Rectangle GetRect() {
		return Rectangle{ x - width / 2, y - height / 2, width, height };
	}

	void Draw() {
		DrawRectangleRec(GetRect(), WHITE);
	}
};

int main() {
	InitWindow(0, 0, "Pong");
	SetWindowState(FLAG_VSYNC_HINT);
	ToggleFullscreen();

	Ball ball;
	ball.x = GetScreenWidth() / 2;
	ball.y = GetScreenHeight() / 2;
	ball.radius = 8;
	ball.speedX = 500;
	ball.speedY = 500;

	Paddle leftPaddle;
	leftPaddle.x = 50;
	leftPaddle.y = GetScreenHeight() / 2;
	leftPaddle.width = 15;
	leftPaddle.height = 200;
	leftPaddle.speed = 700;

	Paddle rightPaddle;
	rightPaddle.x = GetScreenWidth() - 50;
	rightPaddle.y = GetScreenHeight() / 2;
	rightPaddle.width = 15;
	rightPaddle.height = 200;
	rightPaddle.speed = 700;

	const char* winner = nullptr;
	const char* restartText = "Press SPACE to restart";

	while (!WindowShouldClose()) {

		ball.x += ball.speedX * GetFrameTime();
		ball.y += ball.speedY * GetFrameTime();

		if (ball.y < 0) {
			ball.y = 0;
			ball.speedY *= -1;
		}
		if (ball.y > GetScreenHeight()) {
			ball.y = GetScreenHeight();
			ball.speedY *= -1;
		}
		if (ball.x < 0) {
			winner = "Right Player Wins!";
		}
		if (ball.x > GetScreenWidth()) {
			winner = "Left Player Wins!";
		}

		if (IsKeyDown(KEY_W)) {
			leftPaddle.y -= leftPaddle.speed * GetFrameTime();
		}
		else if (IsKeyDown(KEY_S)) {
			leftPaddle.y += leftPaddle.speed * GetFrameTime();
		}

		if (IsKeyDown(KEY_UP)) {
			rightPaddle.y -= rightPaddle.speed * GetFrameTime();
		}
		else if (IsKeyDown(KEY_DOWN)) {
			rightPaddle.y += rightPaddle.speed * GetFrameTime();
		}

		if (CheckCollisionCircleRec(Vector2{ ball.x, ball.y }, ball.radius, leftPaddle.GetRect())) {
			if (ball.speedX < 0) {
				ball.speedX *= -1.04;
				ball.speedY = (ball.y - leftPaddle.y) / (leftPaddle.height / 2) * ball.speedX;
			}
		}
		if (CheckCollisionCircleRec(Vector2{ ball.x, ball.y }, ball.radius, rightPaddle.GetRect())) {
			if (ball.speedX > 0) {
				ball.speedX *= -1.04;
				ball.speedY = (ball.y - rightPaddle.y) / (rightPaddle.height / 2) * -ball.speedX;
			}
		}

		if (winner && IsKeyPressed(KEY_SPACE)) {
			ball.x = GetScreenWidth() / 2;
			ball.y = GetScreenHeight() / 2;
			ball.speedX = 500;
			ball.speedY = 500;
			winner = nullptr;
		}

		BeginDrawing();
		ClearBackground(BLACK);

		ball.Draw();
		leftPaddle.Draw();
		rightPaddle.Draw();

		if (winner) {
			int winnerTextWidth = MeasureText(winner, 60);
			int restartTextWidth = MeasureText(restartText, 25);
			DrawText(winner, GetScreenWidth() / 2 - winnerTextWidth / 2, GetScreenHeight() / 2 - 30, 60, YELLOW);
			DrawText(restartText, GetScreenWidth() / 2 - restartTextWidth / 2, GetScreenHeight() / 2 + 80, 25, WHITE);
		}

		DrawFPS(10, 10);
		EndDrawing();

	}

	CloseWindow();

	return 0;
}