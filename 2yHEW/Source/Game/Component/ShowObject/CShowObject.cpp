#include "CShowObject.h"

void CShowObject::Show()
{
	m_isVisible = true;
}

void CShowObject::Hide()
{
	m_isVisible = false;
}

void CShowObject::SetVisible(bool isVisible)
{
	m_isVisible = isVisible;
}

bool CShowObject::IsVisible() const
{
	return m_isVisible;
}
