#include "DragFloat3.h"
#include "imgui.h"

Pg::UI::Widget::DragFloat3::DragFloat3(std::string label, Pg::Math::PGFLOAT3* input, float min)
	: _label("##"), _inputIndex(0), _inputPGFloat3(input), _minFloat(min)
{
	_label.append(label);

	_inputFloat[0] = _inputPGFloat3->x;
	_inputFloat[1] = _inputPGFloat3->y;
	_inputFloat[2] = _inputPGFloat3->z;
}

Pg::UI::Widget::DragFloat3::DragFloat3(std::string label, Pg::Math::PGFLOAT4* input, float min)
	: _label("##"), _inputIndex(1), _inputPGFloat4(input), _minFloat(min)
{
	_label.append(label);

	_inputFloat[0] = _inputPGFloat4->x;
	_inputFloat[1] = _inputPGFloat4->y;
	_inputFloat[2] = _inputPGFloat4->z;
}

Pg::UI::Widget::DragFloat3::DragFloat3(std::string label, Pg::Math::PGQuaternion* input, float min)
	: _label("##"), _inputIndex(2), _inputPGQuat(input), _minFloat(min)
{
	_label.append(label);

	_inputPGFloat3 = new Pg::Math::PGFLOAT3;
	*_inputPGFloat3 = Pg::Math::PGQuaternionToEuler(*_inputPGQuat);

	_inputFloat[0] = Pg::Math::PGConvertToDegrees(_inputPGFloat3->x);
	_inputFloat[2] = Pg::Math::PGConvertToDegrees(_inputPGFloat3->z);
	_inputFloat[1] = Pg::Math::PGConvertToDegrees(_inputPGFloat3->y);
}

void Pg::UI::Widget::DragFloat3::Update()
{
	if (_inputIndex == 0)
	{
		_inputFloat[0] = _inputPGFloat3->x;
		_inputFloat[1] = _inputPGFloat3->y;
		_inputFloat[2] = _inputPGFloat3->z;
	}
	else if (_inputIndex == 1)
	{
		_inputFloat[0] = _inputPGFloat4->x;
		_inputFloat[1] = _inputPGFloat4->y;
		_inputFloat[2] = _inputPGFloat4->z;
	}
	else
	{
		*_inputPGFloat3 = Pg::Math::PGQuaternionToEuler(*_inputPGQuat);

		_inputFloat[0] = Pg::Math::PGConvertToDegrees(_inputPGFloat3->x);
		_inputFloat[2] = Pg::Math::PGConvertToDegrees(_inputPGFloat3->z);
		_inputFloat[1] = Pg::Math::PGConvertToDegrees(_inputPGFloat3->y);
	}

	if (ImGui::DragFloat3(_label.c_str(), _inputFloat, 0.005f, _minFloat))
	{
		if (_inputIndex == 0)
		{
			_inputPGFloat3->x = _inputFloat[0];
			_inputPGFloat3->y = _inputFloat[1];
			_inputPGFloat3->z = _inputFloat[2];
		}
		else if (_inputIndex == 1)
		{
			_inputPGFloat4->x = _inputFloat[0];
			_inputPGFloat4->y = _inputFloat[1];
			_inputPGFloat4->z = _inputFloat[2];
		}
		else
		{
			_inputPGFloat3->x = Pg::Math::PGConvertToRadians(_inputFloat[0]);
			_inputPGFloat3->y = Pg::Math::PGConvertToRadians(_inputFloat[1]);
			_inputPGFloat3->z = Pg::Math::PGConvertToRadians(_inputFloat[2]);

			*_inputPGQuat = Pg::Math::PGEulerToQuaternion(*_inputPGFloat3);
		}
	}
}


