// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/Abilities/GameplayCues/RPGC_Sprint.h"
#include "Kismet/GameplayStatics.h"
#include "Characters/Heros/RPHeroCharacter.h"
#include "Particles/ParticleSystemComponent.h"
#include "Particles/ParticleSystem.h"

ARPGC_Sprint::ARPGC_Sprint()
{

}

void ARPGC_Sprint::ExecuteWhileActive(AActor* MyTarget, const FGameplayCueParameters& Parameters)
{
    ARPHeroCharacter* MC = Cast<ARPHeroCharacter>(MyTarget);

    if (MC)
    {
        USceneComponent* Mesh = MC->GetMesh();

        if (Mesh && SprintFX)
        {
            Particle_L = UGameplayStatics::SpawnEmitterAttached
            (
                SprintFX,
                Mesh,
                AttachPointName_L
            );

            Particle_R = UGameplayStatics::SpawnEmitterAttached
            (
                SprintFX,
                Mesh,
                AttachPointName_R
            );

        }

    }


}

void ARPGC_Sprint::ExecuteOnRemove(AActor* MyTarget, const FGameplayCueParameters& Parameters)
{
    if (Particle_L && Particle_R)
    {
        Particle_L->DestroyComponent();
        Particle_R->DestroyComponent();
    }
}
