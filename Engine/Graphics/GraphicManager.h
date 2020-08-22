#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <list>
#include "ShaderManagment/ShaderManager.h"
#include "DrawData.h"
#include "ShaderManagment/Shader.h"
#include "../Utility/Coordinate.h"
#include "../Debugger/Debugger.h"
#include "FPSCounter.h"

namespace tge {
	//! Структура для хранения информации об изображениях и анимации, используемый ТОЛЬКО в GraphicManager
	struct GraphicPrefab {
		sf::Texture texture; //! sfml текстура префаба
		sf::Sprite sprite; //! sfml спрайт для отрисовки
		Vector2F size; //! размер спрайта в пикселях
		unsigned frames_count = 1; //! Кол-во спрайтов в анимации. Для одиночного изображения 1
	};

	enum BasicShapes {
		Square, Circle
	};
}

//! Структура для загрузки новых изображеней в GraphicManager
struct GraphicPrefabData {
	GraphicPrefabData(std::string f, Vector2F s, unsigned fc) :
		file(f), size(s), frames_count(fc) {}
	std::string file; //! файл, при необходимости путь к нему
	Vector2F size; //! размер спрайта в пикселях
	unsigned frames_count = 1; //! Кол-во спрайтов в анимации. Для одиночного изображения 1
};

/*!
* Статичный класс, предназначенный для работы с графикой.
*
* Единсвенная функция, которая может быть вызвана в игровом коде - функция Draw, чтобы нарисовать данный спрайт,
* об остальном система позаботиться за вас.
*
* Это один из немногих модулей, зависимых от SFML (остальное, в теории, может использоваться без него).
* Если появится необходимость пересесть с SFML на что-то другое, нужно будет переписать данный класс
* (а также TimeManager и InputManager), сохранив назначение функций (тогда не потребуется полное переписывание остального кода)
*
* Спрайты хранятся непосредственно внутри данного класса. Объекты хранят только ID своих спрайтов в виде enum Sprites
*
* Отрисовка зависит от DrawData и View.
* DrawData - информация о том, что, как и где нужно отрисовать (подробно в соответствующем файле)
* View отвечает за постобработку.
* Используя View можно не заботиться о попиксельном расчёте положений структур. Подробнее о том как работает View находится в DrawData.h
*
* СИСТЕМНОЕ
*/
class GraphicManager
{
public:

	//! Инициализация, проходит автоматически
	static void Init();

	//! Обновление происходит каждый кадр
	static bool Update();

	//! Выход вызывается при закрытии окна. Схлопывает всю игру, приводит к её завершению
	static void Exit();

	//static void LoadPack();
	//! Нарисовать спрайт согласно DrawData, применив к нему View и view_id. data при этом необратимо портится!!!
	//! Вернёт true, если нарисовано успешно, false если нет (id спрайта больше чем число загруженных спрайтов)
	static bool Draw(DrawData& data, unsigned view_id = 0);

	//! Применяет к data соответствующий View. При этом data изменяется!
	static void SetView(DrawData& data, unsigned view_id);

	//! Устанавливает максимальное количество одновременно загруженных спрайтов
	//! Вернёт true при успешном расширении, false иначе (например, если число меньше
	//! кол-ва спрайтов, загруженных движком по умолчанию)
	static bool SetSpritesMaxCount(unsigned count);

	//! Получить максимальное количество одновременно загруженных спрайтов
	static unsigned GetSpritesMaxCount();

	//! Получить количество загруженных спрайтов на данных момент
	static unsigned GetSpritesCount();

	//! Освобождает место для спрайтов, сохраняя их максимально возможное количество (не гарантирует мгновенного освобождения памяти) 
	static void ClearSprites();

	/*!
	* Загружает спрайт по заданному пути и помещает его на следующий свободный id
	* Вернёт id нового спрайта при успешной загрузке, вернёт -1 в другом случае (нет свободных id, не найден файл по пути)
	*/
	static int LoadSprite(GraphicPrefabData data);

	/*!
	* Загружает спрайт по заданному пути и помещает его на заданный id.
	* Спрайт, который находился ранее по этому id перезаписывается
	* Вернёт true при успешной загрузке, иначе вернёт false (данный id больше максимальное возможного, не найден файл по пути)
	* id <= GetSpritesMaxCount() ВСЕГДА
	*/
	static bool LoadSprite(GraphicPrefabData& data, unsigned id);

	/*!
	* Возвращает указатель на view, имеющий данное ID.
	* View по данному указателю МОЖНО изменять, при том эти изменения отразятся на отображении всех объектов, использующих данный view
	* Настоятельно не рекомендуется менять параметры real_size и
	*/
	static View* GetView(unsigned view_id);

	/*!
	* Добавит новую view в список доступных view
	* Вернёт id (> 0) данной view, если view добавлена успешно
	* Вернёт -1, если не получилось добавить view
	*/
	static int AddView(View view);

	/*!
	*  Удалит все пользовательские view из списка
	*/
	static void ResetViews();

	//! Переводит позицию чего-либо в системе экрана монитора в систему данного view
	static Vector2F ConvertRealToView(Vector2F pos, unsigned view_id);

	//! Переводит позицию чего-либо в системе экрана монитора в систему данного view
	static Vector2F ConvertRealPosToView(Vector2F pos, unsigned view_id);

	//! Переводит позицию чего-либо в системе данной view в систему экрана монитора
	static Vector2F ConvertViewPosToReal(Vector2F pos, unsigned view_id);

	//! Переводит размер чего-либо из системы данной view в систему экрана монитора. Применять только если отношения сторон real_size и virtual_size совпадают!!!
	static float ConvertViewSizeToReal(float len, unsigned view_id);

	//! Вернёт указатель на сфмл-окно (Не советую использовать вообще никогда)
	static sf::RenderWindow* GetWindow();

	//! Устанавливает новое разрешение экрана в пикселях
	//! Меняет view по умолчанию так, чтобы она соответствовала новому разрешению
	//! НЕ МЕНЯЕТ view, добавленные до этого, от чего они могут перестать работать корректно
	//! Советуется делать ResetViews() и загружать новые согласно новому разрешению
	static void SetResolution(Vector2U new_size);

	//! Вернёт разрешение экрана в пикселях
	static Vector2U GetResolution();

	//! Устанавливает, необходимо ли отправлять в консоль данные о текущем ФПС
	static void ShowFPS(bool is_show);

	//! Задаёт шейдер, который будет применяться к данному слою
	//! Шейдер слоя применяется после шейдеров данного спрайта
	//! Экземпляр шейдера должен храниться где-то, менеджер не обеспечивает очищение памяти
	//! Для отключение шейдера отправлять shader = nullptr
	//! Если слой больше максимального кол-ва слоёв, не сделает ничего
	//! Шейдер, применённый к слою, работает в разы быстрее, чем если бы вы применяли шейдер к каждому спрайту на слое
	static void SetLayerShader(unsigned layer, Shader* shader);

	//! Устанавливает кол-во слоёв, на которых возможно рисование
	//! При уменьшении кол-ва слоёв, все данные в удалённых слоях пропадают
	//! В том числе и привязанные к слою шейдеры
	//! По умолчанию доступно 20 слоёв
	static void SetLayersCount(unsigned count);
private:
	static sf::RenderWindow window; //! Окно SFML, на котором происходит вся отрисовка

	static std::vector<tge::GraphicPrefab> sprites; //! Массив всех спрайтов игры

	static std::vector<tge::GraphicLayer> to_draw; //! Массив всех спрайтов, которые нужно нарисовать в данный кадр

	static std::vector<View> views; //! Массив всех Views.

	static unsigned LAYER_COUNT; //! Число всех слоёв отрисовки. НЕ ПУТАТЬ СО СЛОЯМИ ОБЪЕКТОВ. Отвечает за то, какие спрайты перекроет данный спрайт, а какие нет

	static unsigned _sprites_count;

	static std::vector<int> _basic_shapes;

	static unsigned _engine_sprites_count;

	static tge::FPSCounter _fps_counter;

	friend class Debugger;
};

