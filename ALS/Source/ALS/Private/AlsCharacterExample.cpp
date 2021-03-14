#include "AlsCharacterExample.h"

#include "Components/InputComponent.h"
#include "Utility/AlsMath.h"

void AAlsCharacterExample::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("LookUpMouse", this, &ThisClass::InputLookUpMouse);
	PlayerInputComponent->BindAxis("LookRightMouse", this, &ThisClass::InputLookRightMouse);

	PlayerInputComponent->BindAxis("LookUp", this, &ThisClass::InputLookUp);
	PlayerInputComponent->BindAxis("LookRight", this, &ThisClass::InputLookRight);

	PlayerInputComponent->BindAxis("MoveForward", this, &ThisClass::InputMoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &ThisClass::InputMoveRight);

	PlayerInputComponent->BindAction("Sprint", IE_Pressed, this, &ThisClass::InputSprintPressed);
	PlayerInputComponent->BindAction("Sprint", IE_Released, this, &ThisClass::InputSprintReleased);
	PlayerInputComponent->BindAction("Sprint", IE_DoubleClick, this, &ThisClass::InputRoll);

	PlayerInputComponent->BindAction("Walk", IE_Pressed, this, &ThisClass::InputWalk);
	PlayerInputComponent->BindAction("Crouch", IE_Pressed, this, &ThisClass::InputCrouch);

	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ThisClass::InputJumpPressed);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ThisClass::InputJumpReleased);

	PlayerInputComponent->BindAction("Aim", IE_Pressed, this, &ThisClass::InputAimPressed);
	PlayerInputComponent->BindAction("Aim", IE_Released, this, &ThisClass::InputAimReleased);

	PlayerInputComponent->BindAction("RotateToVelocityDirection", IE_Pressed, this, &ThisClass::VelocityDirectionPressedAction);
	PlayerInputComponent->BindAction("RotateToLookDirection", IE_Pressed, this, &ThisClass::LookingDirectionPressedAction);

	PlayerInputComponent->BindAction("Ragdoll", IE_Pressed, this, &ThisClass::RagdollPressedAction);
}

void AAlsCharacterExample::InputLookUpMouse(const float Value)
{
	AAlsCharacterExample::AddControllerPitchInput(-Value * LookUpMouseSensitivity);
}

void AAlsCharacterExample::InputLookRightMouse(const float Value)
{
	AAlsCharacterExample::AddControllerYawInput(Value * LookRightMouseSensitivity);
}

void AAlsCharacterExample::InputLookUp(const float Value)
{
	AAlsCharacterExample::AddControllerPitchInput(-Value * LookUpRate * GetWorld()->GetDeltaSeconds());
}

void AAlsCharacterExample::InputLookRight(const float Value)
{
	AAlsCharacterExample::AddControllerYawInput(Value * LookRightRate * GetWorld()->GetDeltaSeconds());
}

void AAlsCharacterExample::InputMoveForward(const float Value)
{
	AddMovementInput(FVector{UAlsMath::AngleToDirection(GetAimingState().SmoothRotation.Yaw), 0.0f},
	                 UAlsMath::FixGamepadDiagonalValues(Value, GetInputAxisValue("MoveRight")));
}

void AAlsCharacterExample::InputMoveRight(const float Value)
{
	AddMovementInput(FVector{UAlsMath::AngleToDirection(GetAimingState().SmoothRotation.Yaw + 90.0f), 0.0f},
	                 UAlsMath::FixGamepadDiagonalValues(Value, GetInputAxisValue("MoveForward")));
}

void AAlsCharacterExample::InputSprintPressed()
{
	SetDesiredGait(EAlsGait::Sprinting);
}

void AAlsCharacterExample::InputSprintReleased()
{
	SetDesiredGait(EAlsGait::Running);
}

void AAlsCharacterExample::InputRoll()
{
	TryStartRolling(1.3f);
}

void AAlsCharacterExample::InputWalk()
{
	// ReSharper disable once CppDefaultCaseNotHandledInSwitchStatement
	// ReSharper disable once CppIncompleteSwitchStatement
	switch (GetDesiredGait())
	{
		case EAlsGait::Walking:
			SetDesiredGait(EAlsGait::Running);
			break;

		case EAlsGait::Running:
			SetDesiredGait(EAlsGait::Walking);
			break;
	}
}

void AAlsCharacterExample::InputCrouch()
{
	switch (GetDesiredStance())
	{
		case EAlsStance::Standing:
			SetDesiredStance(EAlsStance::Crouching);
			break;

		case EAlsStance::Crouching:
			SetDesiredStance(EAlsStance::Standing);
			break;
	}
}

void AAlsCharacterExample::InputJumpPressed()
{
	if (TryStopRagdolling())
	{
		return;
	}

	if (TryStartMantlingGrounded())
	{
		return;
	}

	if (GetStance() == EAlsStance::Crouching)
	{
		SetDesiredStance(EAlsStance::Standing);
		return;
	}

	Jump();
}

void AAlsCharacterExample::InputJumpReleased()
{
	StopJumping();
}

void AAlsCharacterExample::InputAimPressed()
{
	SetAiming(true);
}

void AAlsCharacterExample::InputAimReleased()
{
	SetAiming(false);
}

void AAlsCharacterExample::VelocityDirectionPressedAction()
{
	SetDesiredRotationMode(EAlsRotationMode::VelocityDirection);
}

void AAlsCharacterExample::LookingDirectionPressedAction()
{
	SetDesiredRotationMode(EAlsRotationMode::LookingDirection);
}

void AAlsCharacterExample::RagdollPressedAction()
{
	if (!TryStopRagdolling())
	{
		StartRagdolling();
	}
}
