type magnitude =
  | Liquid
  | Solid;

type measurementUnits =
  | Metric
  | Imperial;

type ingridient = {
  name: string,
  density: float,
  image: BsReactNative.Packager.required,
  unit: magnitude,
  uuid: int,
};

let ingridients = [|
  {
    uuid: 0,
    name: "Milk",
    density: 237.0,
    image: BsReactNative.Packager.require("../../../assets/milk.png"),
    unit: Liquid,
  },
  {
    uuid: 1,
    name: "Flour",
    density: 128.0,
    image: BsReactNative.Packager.require("../../../assets/flour.png"),
    unit: Solid,
  },
  {
    uuid: 2,
    name: "Butter",
    density: 227.0,
    image: BsReactNative.Packager.require("../../../assets/butter.png"),
    unit: Solid,
  },
  {
    uuid: 3,
    name: "Baking Soda",
    density: 180.0,
    image: BsReactNative.Packager.require("../../../assets/baking_soda.png"),
    unit: Solid,
  },
  {
    uuid: 4,
    name: "Cocoa",
    density: 125.0,
    image: BsReactNative.Packager.require("../../../assets/cocoa.png"),
    unit: Solid,
  },
  {
    uuid: 5,
    name: "Cream",
    density: 237.0,
    image: BsReactNative.Packager.require("../../../assets/cream.png"),
    unit: Liquid,
  },
  {
    uuid: 6,
    name: "Oil",
    density: 224.0,
    image: BsReactNative.Packager.require("../../../assets/oil.png"),
    unit: Solid,
  },
  {
    uuid: 7,
    name: "Powdered Sugar",
    density: 128.0,
    image: BsReactNative.Packager.require("../../../assets/pow_sugar.png"),
    unit: Solid,
  },
  {
    uuid: 8,
    name: "Sugar",
    density: 201.0,
    image: BsReactNative.Packager.require("../../../assets/sugar.png"),
    unit: Solid,
  },
  /* THERE SHOULD BE NO NUMBER 9 */
  {
    uuid: 9,
    name: "Milk",
    density: 0.14,
    image: BsReactNative.Packager.require("../../../assets/milk.png"),
    unit: Liquid,
  },
|];

let find = uuid =>
  List.find(
    ingridient => ingridient.uuid == uuid,
    Array.to_list(ingridients),
  );

let lenght = List.length(Array.to_list(ingridients));

type metaData = {
  imageResource: BsReactNative.Packager.required,
  coordinates: list(float),
};

let getForgroundData = (step, w) => {
  let even = step mod 2 == 0 ? true : false;
  let data: metaData = {
    imageResource: find(even ? step : step + 1).image,
    coordinates: even ? [0., 1. *. w] : [1. *. w, 0.],
  };
  data;
};

let getBackgroundData = (step, w) => {
  let even = step mod 2 == 0 ? true : false;
  let data: metaData = {
    imageResource: find(! even ? step : step + 1).image,
    coordinates: even ? [0., (-1.) *. w] : [(-1.) *. w, 0.],
  };
  data;
};