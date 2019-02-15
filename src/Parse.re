let message = (step: int, extra: int) => {
  let message =
    switch (step) {
    | 0 => extra > 0 ? string_of_int(extra) : "0"
    | 1 => extra > 0 ? string_of_int(extra) ++ {j|¼|j} : {j|¼|j}
    | 2 => extra > 0 ? string_of_int(extra) ++ {j|⅓|j} : {j|⅓|j}
    | 3 => extra > 0 ? string_of_int(extra) ++ {j|½|j} : {j|½|j}
    | 4 => extra > 0 ? string_of_int(extra) ++ {j|⅔|j} : {j|⅔|j}
    | 5 => extra > 0 ? string_of_int(extra) ++ {j|¾|j} : {j|¾|j}
    | 6 => extra > 0 ? string_of_int(extra + 1) : "1"
    };
  message;
};

let convertedMagnitude =
    (
      ~step: int,
      ~extraCups: int,
      ~ingridientUuid: int,
      ~unit: Ingridients.measurementUnits,
    ) => {

      let valo = "lol";
      valo;
  let ingridient = Ingridients.find(ingridientUuid);
  let cupFraction =
    switch (step) {
    | 0 => 0.
    | 1 => 0.25
    | 2 => 0.33
    | 3 => 0.5
    | 4 => 0.66
    | 5 => 0.75
    | 6 => 1.
    };

  let converted =
    switch (ingridient.unit) {
    | Liquid =>
      switch (unit) {
      | Ingridients.Metric =>
        floor((cupFraction +. float_of_int(extraCups)) *. ingridient.density)
      | Ingridients.Imperial =>
        (cupFraction +. float_of_int(extraCups))
        *. ingridient.density
        /. 29.57
      }
    | Solid =>
      switch (unit) {
      | Ingridients.Metric =>
        floor((cupFraction +. float_of_int(extraCups)) *. ingridient.density)
      | Ingridients.Imperial =>
        (cupFraction +. float_of_int(extraCups))
        *. ingridient.density
        /. 28.35
      }
    };

  let fixed = float_of_string(Js.Float.toFixedWithPrecision(converted, ~digits=1));

  if (mod_float(fixed, 1.) > 0.) {
    string_of_float(fixed);
  } else {
    string_of_int(int_of_float(fixed));
  }
};

/* let find = uuid =>
   List.find(
     ingridient => ingridient.uuid == uuid,
     Array.to_list(ingridients),
   ); */