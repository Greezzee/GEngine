Данный файл имеет подробные описания коммитов, расписанные по датам.

Коммиты от 25.07.20:
- На основе существующего кода был создать проект Visual Studio. Исходный код НИКАК НЕ МЕНЯЛСЯ. Всё работает стабильно.
К проекту был подключён SFML версии 2.5.1 для 32-битных систем. Этот же sfml был подключён к проекту. 
Добавились папки Debuf и Realese, в котором лежат собранные на данный момент элементы проекта (объектники, .dll и .exe файлы). Эти папки не удалять, они важны для VS.
Добавлен .gitignore для успешных коммитов файлов проекта visual studio.
Изменения не должны были никак сказаться на работоспособности кода на других системах. 
- произошёл откат предлагаемых изменений в GameManager. Теперь основной файл игры (в вашем случае Project_terma) - сцена.
Добавлен файл Main.cpp, где находится функция int main() и не должно находиться НИЧЕГО, кроме неё. В нём же вы можете указать, какая сцена стартовая.
В функцию GameManager::Launch() добавлен новый параметр типа Scene*. Это указатель на уже созданную, проинициализированную главную сцену.
Она запускается первой.
Файлы GameManager были перенесены из папки Gameplay в новую папку GameManagment, так как до этого содержимое папки Gameplay не было единообразным.
- добавлены readme файлы, рекомендую ознакомиться.
- Теперь перед запуском GameManager::Launch() необходим запуск GameManager::Init() перед этим.
Немного изменена механика GraphicManager. Теперь есть возможность манипуляции библиотекой текстур, заложенной в GraphicManager в процессе игры.
Сильно советую прочитать комментарии к функциям GraphicManager, а также посмотреть пример, который реализован в сцене ProjectTerma
(загрузка текстуры, её рисование)

Коммиты от 28.07.20:
- Изменения в коллайдерах. Вместо старого Collider теперь два класса: EllipseCollider и SquareCollider. 
EllipseCollider работает аналогично старому Collider. Из него удалена функция IsCollideWith. Теперь необходимо использовать статичную функция IsCollide() в Collider.
Добавлен SquareCollider. Работает как коллайдер в форме параллелограмма. В Collider была добавлена перегрузка IsCollide() для квадратных коллайдеров.
Советую ознакомиться с комментариями в файле SquareCollider.
TODO: Добавить перегрузку IsCollide() для SquareCollider и EllipseCollider (пересечение коллайдеров разных типов)
Добавлен небольшой механизм работы с прямыми и отрезками (Класс Line в Line.h и некоторые функции). Там только самые необходимые функции, можно их дополнять
- Обновлены View. Удалено поле position, теперь его роль исполняет real_position. Добавлены поля virtual_postition и unit_vector.
virtual position - положение "центра" view в игровых внутренних координатах
unit_vector - направления осей координат. Также он косвенно определяет "центр" view. Принимает только 4 различных значения:
{1, 1} - значение по умолчанию. Задаёт оси координат с центром в верхнем левом углу view. Ось y направлена вниз. Ось x направлена вправо
{1, -1} - Декартова система координат. Задаёт оси координат с центром в нижнем левом углу view. Ось y направлена вверх. Ось x направлена вправо
{-1, 1} - Задаёт оси координат с центром в верхнем правом углу view. Ось y направлена вниз. Ось x направлена влево
{-1, -1} - Задаёт оси координат с центром в нижнем правом углу view. Ось y направлена вверх. Ось x направлена влево


Коммиты от 30.07.20:
- Переработана система загрузки и отрисовки изображений. Теперь чтобы добавить новую текстуру в GraphicManager с помощью LoadTexture, необходимо передать не путь к изображению, а новую структуру.
Структура содержит путь изображения, размер изображение в пикселях и длину анимации в кадрах.
Структура DrawData для функции Draw изменена. Теперь в ней нет поля scale, зато есть поле size, устанавливает размер текстуры в единицах координат поля соответствующего view.
Также теперь origin считается не в пикселях, а как числа от 0 до 1, где (0, 0) - центр спрайта в левом верхнем углу, (1, 1) - в нижнем правом. (0.5, 0.5) - центр находится в геометрическом центре спрайта
Внутренние изменения в GraphicManager, которые вам не должны быть особо интересны
Добавлены анимации! Переменная frame в DrawData теперь имеет смысл! Она показывает на котором кадре находится анимация. При том позволительно любое положительное целое число.
Если это число больше общего кол-ва кадров в анимации, будет нарисован кадр, равный остатку от деления этого числа на кол-во кадров.
ПРАВИЛА РИСОВАНИЯ АНИМИРОВАННЫХ ТЕКСТУР:
Единственный файл изображения. В высоту всегда один кадр, в длину - сколько захотите кадров. Анимация идёт слева направо, перебирая кадры.
Если в процессе загрузки LoadTexture вы укажете неправильные значения и неправильные кол-во кадров анимации, то могут наблюдаться ошибки в виде обрезания спрайта, не до конца играющих анимаций и прочего.
Для тестирования были добавлены файлы StoneBlock и AnimatedBlock

Коммиты от 31.07.20
- Теперь "центр" любого view находится в геометрическом центре игровой зоны. virtual_position задаёт положение центра view в игровых координатах, а не угла

Коммиты от 3.08.20:
- В InputManager добавлены функции управления мышью. 
В GraphicManager добавлена функция, переводящая координаты точек из системы координат относительно окна игры (что аналогично Basic view) в систему
координат любого view, что полезно использовать при получении позиции мыши в системе view
К сожалению, теперь GraphicManager и InputManager связаны друг с другом и я не знаю, как это исправить

Коммиты от 4.08.20:
- СНОВА чуть-чуть переписаны вьюшки. Добавлены новые параметры real_origin и virtual_origin. Они отвечают за смещение "центра" вьюшки
в игровых координатах (virtual_origin) и в системе окна (real_origin). По прежнему ноль по умолчанию (при virtual_origin = {0, 0}) расчитывается
из направлений осей координат. Значение virtual_origin = {1, 1} перенесёт "центр" в противоположный угол экрана. virtual_origin = {0.5, 0.5} перенесёт
центр в геометрический центр в игровых координатах.
Также ЗАПРЕЩЕНЫ нахер растягивания экрана
Заменены все random() на rand(), так как функция random() работает только под g++, а rand() универсальный и работает под любыми системами
- Добавлена функция, которая измеряет расстояние между двумя коллайдерами при движении первого в заданном направлении. Пока работает только для SquareCollider.
Плюсом были добавлены несколько технических функций, а также скалярное произведение для Vector2F

Коммиты от 5.08.20:
- Добавлен класс Debugger, который будет использоваться для отладки. Умеет рисовать точки, линии и коллайдеры
Этот файл будет обновляться один раз в день, даже если коммитов в этот день было сделано несколько. Отдельные коммиты выделены минусом.