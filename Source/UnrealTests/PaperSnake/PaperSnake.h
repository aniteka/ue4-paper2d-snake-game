#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "PaperTileLayer.h"
#include "PaperSnake.generated.h"


USTRUCT(BlueprintType)
struct FMyIntVector2d
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		int32 X;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		int32 Y;
};

UENUM(BlueprintType)
enum class EDir : uint8
{
	UP, DOWN, LEFT, RIGHT 
};


UCLASS()
class UNREALTESTS_API APaperSnake : public APawn
{
	GENERATED_BODY()

public:
	APaperSnake();
protected:
	virtual void BeginPlay() override;


	const float TILE_WIDTH = 50.f;
	const float TILE_HEIGHT = 50.f;

public:	
	FTimerHandle MainTimer;
	UPROPERTY(BlueprintReadOnly, EditInstanceOnly, Category = "Snake")
		float DeltaSeconds = 1.f;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	void Update();
	void Move();
	void Draw();
	void AppleCheck();
	void WallCheck();

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Camera")
		class UCameraComponent* Camera;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Camera")
		class USpringArmComponent* CameraArm;


	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Snake")
		class UPaperTileMapComponent* TileMap;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Snake")
		TArray<FMyIntVector2d> Snake;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Snake")
		FMyIntVector2d Apple;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Snake")
		EDir Dir = EDir::RIGHT;


	UPROPERTY(EditAnywhere, Category = "Color")
		FPaperTileInfo NothingColor;
	UPROPERTY(EditAnywhere, Category = "Color")
		FPaperTileInfo SnakeColor;
	UPROPERTY(EditAnywhere, Category = "Color")
		FPaperTileInfo AppleColor;


	void MoveRight();
	void MoveLeft();
	void MoveUp();
	void MoveDown();
};
