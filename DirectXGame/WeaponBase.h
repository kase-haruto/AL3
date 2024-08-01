#include "Collider.h"
#include"ViewProjection.h"

/// <summary>
/// ハンマー
/// </summary>
class WeaponBase :
    public Collider{
public:
    WeaponBase() = default;
    virtual ~WeaponBase() = default;

    virtual void Initialize() = 0;
    virtual void Draw(const ViewProjection& viewProjection) = 0;


    const Vector3& GetRotation()const;
    const Vector3& GetTranslation()const;

    void SetRotation(const Vector3& rotation);
    void SetTranslation(const Vector3& translation);
    void SetModel(Model* model);

private:
    Model* model_;
    WorldTransform worldTransform_;
};