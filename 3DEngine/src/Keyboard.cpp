#include "Keyboard.h"

bool Keyboard::KeyIsPressed(unsigned char keycode) const noexcept
{
	return m_Keystates[keycode];
}

std::optional<Keyboard::Event> Keyboard::ReadKey() noexcept
{
	if (m_Keybuffer.size() > 0u)
	{
		Keyboard::Event e = m_Keybuffer.front();
		m_Keybuffer.pop();
		return e;
	}
	return {};
}

bool Keyboard::KeyIsEmpty() const noexcept
{
	return m_Keybuffer.empty();
}

std::optional<char> Keyboard::ReadChar() noexcept
{
	if (m_Charbuffer.size() > 0u)
	{
		unsigned char charcode = m_Charbuffer.front();
		m_Charbuffer.pop();
		return charcode;
	}
	return {};
}

bool Keyboard::CharIsEmpty() const noexcept
{
	return m_Charbuffer.empty();
}

void Keyboard::FlushKey() noexcept
{
	m_Keybuffer = std::queue<Event>();
}

void Keyboard::FlushChar() noexcept
{
	m_Charbuffer = std::queue<char>();
}

void Keyboard::Flush() noexcept
{
	FlushKey();
	FlushChar();
}

void Keyboard::EnableAutorepeat() noexcept
{
	autorepeatEnabled = true;
}

void Keyboard::DisableAutorepeat() noexcept
{
	autorepeatEnabled = false;
}

bool Keyboard::AutorepeatIsEnabled() const noexcept
{
	return autorepeatEnabled;
}

void Keyboard::OnKeyPressed(unsigned char keycode) noexcept
{
	m_Keystates[keycode] = true;
	m_Keybuffer.push(Keyboard::Event(Keyboard::Event::Type::Press, keycode));
	TrimBuffer(m_Keybuffer);
}

void Keyboard::OnKeyReleased(unsigned char keycode) noexcept
{
	m_Keystates[keycode] = false;
	m_Keybuffer.push(Keyboard::Event(Keyboard::Event::Type::Release, keycode));
	TrimBuffer(m_Keybuffer);
}

void Keyboard::OnChar(char character) noexcept
{
	m_Charbuffer.push(character);
	TrimBuffer(m_Charbuffer);
}

void Keyboard::ClearState() noexcept
{
	m_Keystates.reset();
}

template<typename T>
void Keyboard::TrimBuffer(std::queue<T>& buffer) noexcept
{
	while (buffer.size() > m_BufferSize)
	{
		buffer.pop();
	}
}