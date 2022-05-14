#include "PaperSnake.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "PaperTileMapComponent.h"
#include "Engine/GameEngine.h"


APaperSnake::APaperSnake()
	: Camera(					CreateDefaultSubobject<UCameraComponent>("Camera"))
	, CameraArm(				CreateDefaultSubobject<USpringArmComponent>("CameraArm"))
	, TileMap(					CreateDefaultSubobject<UPaperTileMapComponent>("TileMap"))
{
	PrimaryActorTick.bCanEverTick = false;

	SetRootComponent(TileMap);
	CameraArm->SetupAttachment(RootComponent);
	Camera->SetupAttachment(CameraArm);

	Snake.Add({ 3, 0 });
	Snake.Add({ 2, 0 });
	Snake.Add({ 1, 0 });
	Apple = { 3,3 };
}
void APaperSnake::BeginPlay()
{
	Super::BeginPlay();

	TileMap->MakeTileMapEditable();

	FMyIntVector2d TileMapSize;
	int32 _;
	TileMap->GetMapSize(TileMapSize.X, TileMapSize.Y, _);
	CameraArm->SetRelativeLocation(FVector(TileMapSize.X / 2 * TILE_WIDTH, 20, -TileMapSize.Y / 2 * TILE_HEIGHT));

	GetWorldTimerManager().SetTimer(
		MainTimer, 
		this, 
		&APaperSnake::Update, 
		DeltaSeconds, 
		true
	);
}




void APaperSnake::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAction("SnakeMoveRight",	IE_Pressed, this, &APaperSnake::MoveRight);
	PlayerInputComponent->BindAction("SnakeMoveLeft",	IE_Pressed, this, &APaperSnake::MoveLeft);
	PlayerInputComponent->BindAction("SnakeMoveUp",		IE_Pressed, this, &APaperSnake::MoveUp);
	PlayerInputComponent->BindAction("SnakeMoveDown",	IE_Pressed, this, &APaperSnake::MoveDown);
}

void APaperSnake::Update()
{
	Move();
	AppleCheck();
	WallCheck();


	Draw();
}

void APaperSnake::Move()
{
	auto i = Snake.CreateIterator();
	i.SetToEnd(); i -= 2;

	for (; i; i--)
	{
		auto j = i;
		j++;
		j->X = i->X;
		j->Y = i->Y;
	}

	switch (Dir)
	{
	case EDir::UP:
		Snake[0].Y--;
		break;
	case EDir::DOWN:
		Snake[0].Y++;
		break;
	case EDir::LEFT:
		Snake[0].X--;
		break;
	case EDir::RIGHT:
		Snake[0].X++;
		break;
	}
}

void APaperSnake::Draw()
{
	FMyIntVector2d TileMapSize;
	int32 _;
	TileMap->GetMapSize(TileMapSize.X, TileMapSize.Y, _);

	for (int32 i = 0; i < TileMapSize.Y; i++)
	{
		for (int32 j = 0; j < TileMapSize.X; j++)
		{
			if (i == Apple.Y && j == Apple.X)
			{
				TileMap->SetTile(j, i, 0, AppleColor);
				continue;
			}

			for (auto& [X, Y] : Snake)
			{
				if (i == Y && j == X)
				{
					TileMap->SetTile(j, i, 0, SnakeColor);
					goto EXIT;
				}
			}

			TileMap->SetTile(j, i, 0, NothingColor);
			EXIT:{}
		}
	}
}

void APaperSnake::AppleCheck()
{
	if (Apple.X == Snake[0].X && Apple.Y == Snake[0].Y)
	{
		FMyIntVector2d TileMapSize;
		int32 _;
		TileMap->GetMapSize(TileMapSize.X, TileMapSize.Y, _);

		Apple = { rand() % TileMapSize.X, rand() % TileMapSize.Y };

		Snake.Add({ -1,-1 });
	}
}

void APaperSnake::WallCheck()
{
	FMyIntVector2d TileMapSize;
	int32 _;
	TileMap->GetMapSize(TileMapSize.X, TileMapSize.Y, _);

	auto& Head = Snake[0];
	if (Head.X < 0) 
		Head.X = TileMapSize.X - 1;
	if (Head.X >= TileMapSize.X) 
		Head.X = 0;
	
	if (Head.Y < 0) 
		Head.Y = TileMapSize.Y - 1;
	if (Head.Y >= TileMapSize.Y) 
		Head.Y = 0;


	auto iter = Snake.CreateIterator();
	iter++;
	for (; iter; iter++)
		if (Head.X == iter->X && Head.Y == iter->Y)
		{
			GEngine->AddOnScreenDebugMessage(-1, 10, FColor::Red,
				FString::Printf(TEXT("%i"), iter.GetIndex())
			);
			Snake.SetNum(iter.GetIndex());
			break;
		}
}


void APaperSnake::MoveRight()
{
	Dir = EDir::RIGHT;
}
void APaperSnake::MoveLeft()
{
	Dir = EDir::LEFT;
}
void APaperSnake::MoveUp()
{
	Dir = EDir::UP;
}
void APaperSnake::MoveDown()
{
	Dir = EDir::DOWN;
}

