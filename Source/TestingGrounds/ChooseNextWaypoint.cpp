// Fill out your copyright notice in the Description page of Project Settings.

#include "ChooseNextWaypoint.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"
#include "PatrolRoute.h"

EBTNodeResult::Type UChooseNextWaypoint::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) {
	// Get Patrol Route
	auto Pawn = OwnerComp.GetAIOwner()->GetPawn();
	auto PatrolRoute = Pawn->FindComponentByClass<UPatrolRoute>();
	if (!ensure(PatrolRoute)) { return EBTNodeResult::Failed; }

	auto PatrolPoints = PatrolRoute->GetPatrolPoints();
	if (PatrolPoints.Num() < 1){
		auto PawnName = Pawn->GetName();
		UE_LOG(LogTemp, Warning, TEXT("No patrol points for %s"), *PawnName);
		return EBTNodeResult::Failed;
	}

	// Get Blackboard Data
	auto BlackboardComp = OwnerComp.GetBlackboardComponent();
	auto Index = BlackboardComp->GetValueAsInt(IndexKey.SelectedKeyName);

	// Set Blackboard Data
	BlackboardComp->SetValueAsObject(WaypointKey.SelectedKeyName, PatrolPoints[Index]);
	BlackboardComp->SetValueAsInt(IndexKey.SelectedKeyName, ((Index + 1) % PatrolPoints.Num()));

	return EBTNodeResult::Succeeded;
}


