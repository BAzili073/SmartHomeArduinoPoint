
//
//Следующий вопрос - как работать с несколькими трубами.
//В библиотеке есть две функции: bool available (void) и bool available (uint8_t *pipe_num).
//Если первая требуется для того, чтобы знать есть ли в буфере единственной объявленной трубы что-то для чтения, то вторая позволяет работать со всеми, но для этого ее необходимо правильно вызывать, а именно:
//int in;               //Переменная для приема информации;
//uint8_t pipe_num;     //Переменная, которая покажет нам из какой трубы достали информацию;
//bool done = false;    //Вспомогательная переменная;
//if ( radio.available(&pipe_num) ) { //Скармливаем функции адрес переменной, которая покажет нам номер трубы;
//    while (!done) {   // Упираемся и
//        done = radio.read( &in, sizeof(in) );   // по адресу переменной in функция записывает принятые данные;
//    }
//}
//Обратим внимание еще на некоторые полезные функции.
//setPayloadSize (uint8_t size); - функция определяет размер буфера обмена. По умолчанию размер буфера 32 байта. Если меняем это значение, то обязательно как на приемнике, так и на передатчике.
//void setRetries (uint8_t delay, uint8_t count); - функция изменяет время повтора и количество попыток передатчика "достучаться" до приемника, пример:
//setRetries (15, 15); - с задержкой 15 мс 15 попыток.
//void setPALevel (rf24_pa_dbm_e level); - устанавливает уровень мощности передатчика, и может принимать значения RF24_PA_MIN, RF24_PA_LOW, RF24_PA_HIGH, RF24_PA_MAX. Пример:
//setPALevel (RF24_PA_MAX); - максимальный уровень мощности передатчика.
//bool setDataRate (rf24_datarate_e speed); - одна из трех скоростей, должна быть одинаковой на приемнике и передатчике. Принимает значения RF24_1MBPS, RF24_2MBPS, RF24_250KBPS. 250 кбит занимает более узкую полосу и является наиболее чувствительной скоростью приема. По умолчанию - 1Мбит Пример:
//setDataRate (RF24_250KBPS); ...




//  0,3,2,3,1  настройка светодиода на выход
//  0,3,3,3,1  включение светодиода
//  0,3,3,3,0  выключение светодиода 

//  0,3,$E,$11,21  добавление датчика DHT21
//  0,3,$A,$11,0  get temp DHT21
//  0,3,$C,$11,0  get humi DHT21

//  0,4,2,$11,1  настройка реле на выход
//  0,4,3,$11,1  включение реле
//  0,4,3,$11,0  выключение реле

//  0,4,2,$10,0  настройкадатчика движения на вход
//  0,4,6,$11,1  включение оповещения при сработке датчика движения

