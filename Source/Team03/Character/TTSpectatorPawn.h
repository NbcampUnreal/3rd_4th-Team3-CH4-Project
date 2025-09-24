// TTSpectatorPawn.h

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SpectatorPawn.h"
#include "InputActionValue.h"
#include "TTSpectatorPawn.generated.h"

class UInputMappingContext;
class UInputAction;

UCLASS()
class TEAM03_API ATTSpectatorPawn : public ASpectatorPawn
{
	GENERATED_BODY()
	
public:
	ATTSpectatorPawn();

	virtual void BeginPlay() override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "IA")
	TObjectPtr<UInputAction> SpectatorPreViewTarget;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "IA")
	TObjectPtr<UInputAction> SpectatorNextViewTarget;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "IMC")
	TObjectPtr<UInputMappingContext> SpectatorIMC;

private:
	UFUNCTION()
	void OnSpectatorPreViewTarget(const FInputActionValue& Value);

	UFUNCTION()
	void OnSpectatorNextViewTarget(const FInputActionValue& Value);

};
