open BsReactNative;

type state = {
  barHeight: float,
  deltaValue: float,
  value: float,
  step: int,
  horizontalStep: int,
};
type action =
  | SetBarHeight(float)
  | UpdateDeltaValue(float)
  | SetValue(float)
  | UpdateStep(int)
  | UpdateHorizontalStep(int);
type step = {
  offset: float,
  value: int,
};

let circleDiameter = 35.;

let component = ReasonReact.reducerComponent("VerticalSlider");

type rangesVal = {
  min: int,
  max: int,
  steps: int,
};
let ranges = {min: 0, max: 100, steps: 6};

let getBottomOffsetFromValue =
    (~value: int, ~rangeMin: int, ~rangeMax: int, ~barHeight: float) =>
  float_of_int(value - rangeMin)
  /. float_of_int(rangeMax - rangeMin)
  *. barHeight;

let getValueFromBottomOffset =
    (~offset: float, ~rangeMin: int, ~rangeMax: int, ~barHeight: float) =>
  float_of_int(rangeMax - rangeMin) *. offset /. barHeight;

let capValue = (~value: float, ~min: int, ~max: int) =>
  value <= float_of_int(min) ?
    min : value >= float_of_int(max) ? max : int_of_float(value);

let calculateStepStateFromOffset =
    (~steps: int, ~barHeight: float, ~value: float)
    : step => {
  let t = barHeight /. float_of_int(steps);

  let roundedStep = int_of_float(value /. t);

  let newStepState = {
    offset: float_of_int(roundedStep) *. t,
    value: roundedStep,
  };

  newStepState;
};

let generateStepMarkers = (~steps: int, ~barHeight: float) => {
  let offset = barHeight /. float_of_int(steps);

  let offsets: array(float) = Array.make(steps - 1, 0.);
  for (i in 0 to steps - 2) {
    offsets[i] =
      float_of_int(i + 1) *. offset;
      /* Js.log("Item " ++ string_of_int(i) ++ " with offset " ++ string_of_float(offsets[i])); */
  };

  let markers =
    List.map(
      offsetItem =>
        <View
          key=(string_of_float(offsetItem))
          style=Style.(
                  style([
                    borderRadius(5.),
                    marginBottom(Pt(10.)),
                    backgroundColor(String("#867666")),
                    position(Absolute),
                    height(Pt(3.5)),
                    width(Pt(23.)),
                    bottom(Pt(offsetItem +. circleDiameter /. 4.)),
                  ])
                )
        />,
      Array.to_list(offsets),
    );

  markers;
};

let make = (~onStepMet, ~onHorizontalStep, ~hasFocus, _children) => {
  ...component,
  initialState: () => {
    barHeight: 0.,
    deltaValue: 0.,
    value: 0.,
    step: 0,
    horizontalStep: 0,
  },
  reducer: (action, state) =>
    switch (action) {
    | SetBarHeight(v) => ReasonReact.Update({...state, barHeight: v})
    | UpdateDeltaValue(d) => ReasonReact.Update({...state, deltaValue: d})
    | SetValue(v) => ReasonReact.Update({...state, value: v})
    | UpdateStep(s) => ReasonReact.Update({...state, step: s})
    | UpdateHorizontalStep(s) =>
      ReasonReact.Update({...state, horizontalStep: s})
    },
  render: self => {
    let stepState =
      calculateStepStateFromOffset(
        ~value=
          getBottomOffsetFromValue(
            ~value=
              capValue(
                ~value=self.state.value,
                ~min=ranges.min,
                ~max=ranges.max,
              ),
            ~rangeMin=ranges.min,
            ~rangeMax=ranges.max,
            ~barHeight=self.state.barHeight,
          ),
        ~barHeight=self.state.barHeight,
        ~steps=ranges.steps,
      );

    /*Parent view*/
    <View style=Style.(style([alignItems(Center), justifyContent(Center)]))>
      /*Bar container*/

        <View
          style=Style.(
                  style([
                    flex(1.),
                    alignItems(Center),
                    marginHorizontal(Pt(20.)),
                    paddingVertical(Pt(circleDiameter /. 2.)),
                  ])
                )>
          <View
            /*Bar*/
            onLayout=(
              event => {
                let layoutData = RNEvent.NativeLayoutEvent.layout(event);
                self.send(SetBarHeight(layoutData.height));
              }
            )
            style=Style.(
                    style([
                      flexGrow(1.),
                      backgroundColor(String("#E1B80D")),
                      width(Pt(10.)),
                      borderRadius(10.),
                    ])
                  )
          />
          <View
            /*Circle*/
            responderHandlers=(
              PanResponder.panHandlers(
                PanResponder.create(
                  ~onMoveShouldSetPanResponderCapture=
                    PanResponder.callback((_event, _gesture) => {
                      hasFocus(true);
                      Js.log("bar has focus!");
                      true;
                    }),
                  ~onPanResponderMove=
                    `callback(
                      PanResponder.callback((_event, _gesture) => {
                        let newDeltaValue =
                          getValueFromBottomOffset(
                            ~offset=-. _gesture.dy,
                            ~rangeMin=ranges.min,
                            ~rangeMax=ranges.max,
                            ~barHeight=self.state.barHeight,
                          );

                        self.send(
                          SetValue(self.state.value +. newDeltaValue),
                        );

                        if (stepState.value !== self.state.step) {
                          self.send(UpdateStep(stepState.value));
                          onStepMet(stepState.value);
                        };

                        let horizontalStep =
                          (
                            int_of_float(_gesture.moveX)
                            - int_of_float(_gesture.moveX)
                            mod 100
                          )
                          / 100;
                        if (horizontalStep !== self.state.horizontalStep) {
                          Js.log("Step- " ++ string_of_int(horizontalStep));
                          let corrected =
                            horizontalStep <= 2 ? horizontalStep : 2;
                          self.send(UpdateHorizontalStep(corrected));
                          onHorizontalStep(corrected);
                        };
                      }),
                    ),
                  ~onPanResponderRelease=
                    PanResponder.callback((_event, _gesture) =>
                      hasFocus(false)
                    ),
                  (),
                ),
              )
            )
            style=Style.(
                    style([
                      bottom(Pt(stepState.offset)),
                      borderRadius(circleDiameter),
                      width(Pt(circleDiameter)),
                      height(Pt(circleDiameter)),
                      position(Absolute),
                      backgroundColor(String("#867666")),
                    ])
                  )
          />
          (
            self.state.barHeight > 0. ?
              ReasonReact.array(
                Array.of_list(
                  generateStepMarkers(
                    ~steps=ranges.steps,
                    ~barHeight=self.state.barHeight,
                  ),
                ),
              ) :
              <View />
          )
        </View>
        <View>
          <Text
            style=Style.(style([fontSize(Float(15.)), textAlign(Center)]))
            value="Cup"
          />
          <Text
            style=Style.(style([fontSize(Float(15.))]))
            value="Fractions"
          />
        </View>
      </View>;
  },
};

/*
 #99badd => Karolina blue <3
  */