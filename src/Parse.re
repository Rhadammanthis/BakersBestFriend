let message = (step: int) => {
  let message =
    switch (step) {
    | 0 => "0"
    | 1 => {j|¼|j}
    | 2 => {j|⅓|j}
    | 3 => {j|½|j}
    | 4 => {j|⅔|j}
    | 5 => {j|¾|j}
    | 6 => "1"
    };
  message;
};

let value = (step: int, density: float, unit: float) => {
  let value =
    switch (step) {
    | 0 => 0.
    | 1 => 0.25
    | 2 => 0.3
    | 3 => 0.5
    | 4 => 0.6
    | 5 => 0.75
    | 6 => 1.
    };
    value *. density *. unit;
};

/* let find = uuid =>
   List.find(
     ingridient => ingridient.uuid == uuid,
     Array.to_list(ingridients),
   ); */