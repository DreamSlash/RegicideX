
//#include "RGX_ExecutionFullHeal.h"
//#include "RegicideX/GAS/AttributeSets/RGX_CombatAttributeSet.h"
//#include "RegicideX/GAS/AttributeSets/RGX_HealthAttributeSet.h"
//
//struct RGX_HealthStatics
//{
//	DECLARE_ATTRIBUTE_CAPTUREDEF(Health)
//	DECLARE_ATTRIBUTE_CAPTUREDEF(MaxHealth)
//
//	RGX_HealthStatics()
//	{
//		DEFINE_ATTRIBUTE_CAPTUREDEF(URGX_HealthAttributeSet, Health, Target, true);
//		DEFINE_ATTRIBUTE_CAPTUREDEF(URGX_HealthAttributeSet, MaxHealth, Target, true);
//	}
//};
//
//static const RGX_HealthStatics& HealthStatics()
//{
//	static RGX_HealthStatics healthStatics;
//	return healthStatics;
//}
//
//void URGX_ExecutionFullHeal::Execute_Implementation(
//	const FGameplayEffectCustomExecutionParameters& ExecutionParams, 
//	FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const
//{
//	const FGameplayEffectSpec& Spec = ExecutionParams.GetOwningSpec();
//	const FGameplayEffectContextHandle& ContextHandle = Spec.GetContext();
//	
//	float Health = 0.0f;
//	//ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(HealthStatics().HealthDef, )
//}
