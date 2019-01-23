open BsReactNative;

/* module Styles = {
     open Style;

     let container =
       style([
         flex(1.),
         justifyContent(FlexStart),
         padding(Pt(10.)),
         backgroundColor(String("white")),
       ]);

     let text = style([color(String("#fff")), fontSize(Float(24.))]);
   }; */

type action =
  | TriggerAnim(bool)
  | ResetValue;

type animationReverser = {
  forward: list(float),
  backwards: list(float),
};

type state = {
  shouldAnim: bool,
  triggerAnim: bool,
  translateX: Animated.value(Animated.regular),
};

type retainedProps = {shouldTriggerAnim: bool};

let screenWidth = float(Dimensions.get(`window)##width);
let reverser: animationReverser = {
  forward: [(-150.), 0.],
  backwards: [0., (-150.)],
};

let renderHorizontalStepMarker = (~step: int) =>
  <View
    style=Style.(
            style([
              position(Absolute),
              borderRadius(15.),
              left(Pt(float_of_int(step) *. 100. +. 2.5)),
              top(Pt(24.5)),
              width(Pt(15.)),
              height(Pt(15.)),
              backgroundColor(String("white")),
            ])
          )
  />;

let component = ReasonReact.reducerComponentWithRetainedProps("DropDownBar");

let make = (~shouldTriggerAnim: bool, ~step: int, _children) => {
  ...component,
  initialState: () => {
    shouldAnim: false,
    triggerAnim: false,
    translateX: Animated.Value.create(-150.),
  },
  reducer: (action, state) =>
    switch (action) {
    | ResetValue =>
      ReasonReact.Update({...state, translateX: Animated.Value.create(0.)})
    | TriggerAnim(v) =>
      let animation =
        Animated.timing(
          ~value=state.translateX,
          ~toValue=`raw(1.0),
          ~duration=500.0,
          (),
        );

      Animated.start(animation, Js.log("Complete"));

      ReasonReact.Update({...state, triggerAnim: v});
    },
  retainedProps: {
    shouldTriggerAnim: shouldTriggerAnim,
  },
  didUpdate: ({oldSelf, newSelf}) =>{
    if (oldSelf.retainedProps.shouldTriggerAnim
        !== newSelf.retainedProps.shouldTriggerAnim) {
      /* do whatever sneaky imperative things here */
      newSelf.send(ResetValue);
      newSelf.send(TriggerAnim(newSelf.retainedProps.shouldTriggerAnim));
    }},
  render: self =>
    /* self.send(TriggerAnim(shouldTriggerAnim)); */
    /*Parent view*/
    /* Js.log("inittial: " ++ string_of_bool(shouldTriggerAnim)); */
    <Animated.View
      style=Style.(
              style([
                flex(1.),
                position(Absolute),
                width(Pt(screenWidth)),
                paddingHorizontal(Pt(10.)),
                flexDirection(Column),
                alignItems(Center),
                height(Pt(70.)),
                backgroundColor(String("#867666C9")),
                Transform.makeInterpolated(
                  ~translateY=
                    Animated.Value.interpolate(
                      self.state.translateX,
                      ~inputRange=[0., 1.],
                      ~outputRange=
                        `float(
                          shouldTriggerAnim ?
                            reverser.forward : reverser.backwards,
                        ),
                      (),
                    ),
                  (),
                ),
              ])
            )>
      <Text
        style=Style.(
                style([
                  color(String("white")),
                  fontSize(Float(14.)),
                  paddingRight(Pt(5.)),
                ])
              )
        value="Add just bit more: "
      />
      <View
        style=Style.(
                style([
                  flex(1.),
                  alignItems(Center),
                  marginTop(Pt(-15.)),
                  flexDirection(Row),
                  height(Pt(20.0)),
                  justifyContent(Center),
                ])
              )>
        <View
          style=Style.(
                  style([
                    flex(1.),
                    borderRadius(5.),
                    height(Pt(5.0)),
                    backgroundColor(String("#E1B80D")),
                  ])
                )
        />
        <View
          style=Style.(
                  style([
                    position(Absolute),
                    borderRadius(10.),
                    left(Pt(100.)),
                    top(Pt(22.)),
                    width(Pt(20.)),
                    height(Pt(20.)),
                    backgroundColor(String("#E1B80D")),
                  ])
                )
        />
        <Text
          style=Style.(
                  style([
                    color(String("white")),
                    fontSize(Float(15.)),
                    position(Absolute),
                    left(Pt(100.)),
                    top(Pt(40.)),
                  ])
                )
          value="1/3 "
        />
        <View
          style=Style.(
                  style([
                    position(Absolute),
                    borderRadius(10.),
                    left(Pt(200.)),
                    top(Pt(22.)),
                    width(Pt(20.)),
                    height(Pt(20.)),
                    backgroundColor(String("#E1B80D")),
                  ])
                )
        />
        <Text
          style=Style.(
                  style([
                    color(String("white")),
                    fontSize(Float(15.)),
                    position(Absolute),
                    left(Pt(200.)),
                    top(Pt(40.)),
                  ])
                )
          value="1/4 "
        />
        (step == 1 ? renderHorizontalStepMarker(~step) : step ==2 ? renderHorizontalStepMarker(~step) : <View/>)
      </View>
    </Animated.View>,
};

/*
 #99badd => Karolina blue <3
  */