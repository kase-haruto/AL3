#include "Hammer.h"
#include<cassert>
void Hammer::Initialize(){
	WeaponBase::Initialize();
}

void Hammer::Draw(const ViewProjection& viewProjection){
	WeaponBase::Draw(viewProjection);
}
