`exit` или `:e` - закрыть с сохранением

`oj_sign_in` - повторить процесс входа в `oj.uz`

`save` - сохранить информацию о задачах и турах

`add_problem_to_upsolving "type" "link" "comp"` или `:au "type" "link" "comp"` - добавить задачу в дорешку со сложностью `"comp"`

`add_problem_to_base "type" "link" "comp"` или `:ab "type" "link" "comp"` - добавить задачу, которая может попасть в тур, со сложностью `"comp"`

`search` - найти(возвращает индекс) задачу по:

  1. Имени `-n "name"`

`show_statement "index" "filename"` или `:sh "index" "filename"` - скопировать условие задачи с номером `"index"` в файл `"filename"`

`submit "index" "filename"` или `:s "index" "filename"` - отправить в задачу с индексом `"index"` файл `"filename"`

`check_submission "index" "submission_id"` - выводит информацию о посылке с номером `"submission_id"` по задаче с номерном `"index"`

`problem_info "index"` или `:i "index"` - выводит информацию о задаче с номером `"index"`

`create_tour "opt" "size" "problems"` - создаёт тур с `"size"` задачами(`"problems"`), согласно опции `"opt"`:
  1. `"-r"` - random
  2. `"-c` - custom complexities
  3. `"-fc"` - full custom

`start_tour "index" "time"` - начинает тур с номером `"index"` с продолжительностью `"time"` секунд

`time` - во время тура выводит сколько осталось и сколько прошло

Всё остальное будет работать так же как и в консоли вашей операционной системы
