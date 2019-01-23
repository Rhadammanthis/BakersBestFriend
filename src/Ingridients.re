type ingridient = {
  name: string,
  desnity: float,
  image: BsReactNative.Packager.required,
  unit: string,
  uuid: int,
};

let ingridients = [|
  {
    uuid: 0,
    name: "Milk",
    desnity: 0.14,
    image: BsReactNative.Packager.require("../../../assets/milk.png"),
    unit: "Cups",
  },
  {
    uuid: 1,
    name: "Flour",
    desnity: 0.14,
    image: BsReactNative.Packager.require("../../../assets/flour.png"),
    unit: "Cups",
  },
  {
    uuid: 2,
    name: "Butter",
    desnity: 0.14,
    image: BsReactNative.Packager.require("../../../assets/butter.png"),
    unit: "Cups",
  },
  {
    uuid: 3,
    name: "Baking Soda",
    desnity: 0.14,
    image: BsReactNative.Packager.require("../../../assets/baking_soda.png"),
    unit: "Cups",
  },
  {
    uuid: 4,
    name: "Cocoa",
    desnity: 0.14,
    image: BsReactNative.Packager.require("../../../assets/cocoa.png"),
    unit: "Cups",
  },
  {
    uuid: 5,
    name: "Cream",
    desnity: 0.14,
    image: BsReactNative.Packager.require("../../../assets/cream.png"),
    unit: "Cups",
  },
  {
    uuid: 6,
    name: "Oil",
    desnity: 0.14,
    image: BsReactNative.Packager.require("../../../assets/oil.png"),
    unit: "Cups",
  },
  {
    uuid: 7,
    name: "Powdered Sugar",
    desnity: 0.14,
    image: BsReactNative.Packager.require("../../../assets/pow_sugar.png"),
    unit: "Cups",
  },
  {
    uuid: 8,
    name: "Sugar",
    desnity: 0.14,
    image: BsReactNative.Packager.require("../../../assets/sugar.png"),
    unit: "Cups",
  },
  /* THERE SHOULD BE NO NUMBER 9 */
  {
    uuid: 9,
    name: "Sugar",
    desnity: 0.14,
    image: BsReactNative.Packager.require("../../../assets/sugar.png"),
    unit: "Cups",
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
  coordinates: list(float)
}

let getForgroundData = (step, w) => {
  let even = step mod 2 == 0 ? true : false;
  let data: metaData = {imageResource: find(even ? step : step + 1).image, coordinates: even ? [0., 1. *. w] : [1. *. w,0.]};
  data;
}

let getBackgroundData = (step, w) => {
  let even = step mod 2 == 0 ? true : false;
  let data: metaData = {imageResource: find(!even ? step : step + 1).image, coordinates: even ? [0., -1. *. w] : [-1. *. w,0.]};
  data;
}