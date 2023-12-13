# OSlab4
Лабораторная работа No4.

Тема: “Синхронизация процессов”.

Задача. Написать программу для передачи сообщений между процессами через
общий файл. Программа включает один процесс Receiver и несколько процессов
Sender. Процессы Sender посылают сообщения процессу Receiver.
Требования к реализации процесса Receiver. Процесс Receiver должен
выполнять следующие действия:
1. Ввести с консоли имя бинарного файла и количество записей в бинарном файле,
который будет использоваться для передачи сообщений.
1. Создать бинарный файл для сообщений. Максимальная длина сообщения 20
символов.
2. Ввести с консоли количество процессов Sender.
3. Запустить заданное количество процессов Sender. Каждому процессу Sender
передать через командную строку имя файла для сообщений.
4. Ждать сигнал на готовность к работе от всех процессов Sender.
5. Выполнять циклически следующие действия по команде с консоли:
читать сообщение из бинарного файла или
завершить свою работу.
Замечание. Процесс Receiver переходит в состояние ожидания поступления нового
сообщения, если вызывается команда чтения сообщения из пустого бинарного файла.
Требования к реализации процесса Sender. Процесс Sender должен
выполнять следующие действия:
1. Открыть файл для передачи сообщений. Имя файла получить из командной строки.
2. Отправить процессу Receiver сигнал на готовность к работе.
3. Выполнять циклически следующие действия по команде с консоли:
отправить процессу Receiver сообщение или
прекратить свою работу.
Текст сообщения вводится с консоли. Длина сообщения меньше 20 символов.
Замечание. Процесс Sender переходит в состояние ожидания, если вызывается
команда записи сообщения в заполненный сообщениями бинарный файл.
Дополнительные требования: Передача сообщений через бинарный файл
должна быть организована как кольцевая очередь FIFO. Т.е. процесс Receiver
должен читать сообщения в соответствии с порядком их отправления процессами
Sender.
Упрощенный вариант лабораторной работы. Решить поставленную задачу
для одного процесса Receiver и одного процесса Sender. При этом между
процессами может одновременно передаваться только одно сообщение.
