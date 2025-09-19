// Fill out your copyright notice in the Description page of Project Settings.


#include "Gimmick/TTSpeedBoostItem.h"

ATTSpeedBoostItem::ATTSpeedBoostItem()
{
    SpeedEffect.Additive = 200.f; 
    SpeedEffect.Multiplier = 1.15f; // 15% 상승
    SpeedEffect.Duration = 6.f;   // 6초
}