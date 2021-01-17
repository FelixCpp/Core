#include <Core/Rendering/Targets/TransformationTarget.hpp>
#include <Core/Rendering/RenderStateManager.hpp>
#include <Core/Rendering/RenderState.hpp>

namespace Core
{

	TransformationTarget::TransformationTarget(RenderStateManager *& rsm) :
		rsm(rsm)
	{
	}

	void TransformationTarget::ResetMatrix()
	{
		this->GetState().SetActiveMatrix(D2D1::Matrix3x2F::Identity());
	}

	void TransformationTarget::PushMatrix()
	{
		this->GetState().PushMatrix();
	}

	void TransformationTarget::PopMatrix()
	{
		this->GetState().PopMatrix();
	}

	void TransformationTarget::Translate(float x, float y)
	{
		D2D1::Matrix3x2F & matrix = this->GetState().GetActiveMatrix();
		this->GetState().SetActiveMatrix(matrix * D2D1::Matrix3x2F::Translation(x, y));
	}

	void TransformationTarget::Rotate(float angle)
	{
		D2D1::Matrix3x2F & matrix = this->GetState().GetActiveMatrix();
		this->GetState().SetActiveMatrix(matrix * D2D1::Matrix3x2F::Rotation(angle, D2D1::Point2F(matrix.dx, matrix.dy)));
	}

	void TransformationTarget::Scale(float factorX, float factorY)
	{
		D2D1::Matrix3x2F & matrix = this->GetState().GetActiveMatrix();
		this->GetState().SetActiveMatrix(matrix * D2D1::Matrix3x2F::Scale(factorX, factorY, D2D1::Point2F(matrix.dx, matrix.dy)));
	}

	RenderState & TransformationTarget::GetState()
	{
		return this->rsm->GetActiveState();
	}

}