open BsReactNative;

type action =
  | TriggerAnim(bool)
  | ResetValue
  | SetMessage(string);

type animationReverser = {
  forward: list(float),
  backwards: list(float),
};

type state = {
  onScreen: bool,
  scale: Animated.value(Animated.regular),
  message: string
};

type retainedProps = {step: int};

let reverser: animationReverser = {
    forward: [0., 1.],
    backwards: [1., 0.],
  };

let component =
  ReasonReact.reducerComponentWithRetainedProps("FractionBubble");

let circleDiameter = 100.

let make = (~step: int, _children) => {
  ...component,
  initialState: () => {onScreen: false, scale: Animated.Value.create(1.), message: ""},
  reducer: (action, state) =>
    switch (action) {
    | ResetValue =>
      ReasonReact.Update({...state, scale: Animated.Value.create(0.)})
    | SetMessage(s) => 
    ReasonReact.Update({...state, message: s})
    | TriggerAnim(v) =>
      let animation =
        Animated.timing(
          ~value=state.scale,
          ~toValue=`raw(1.0),
          ~duration=300.0,
          (),
        );

      Animated.start(animation, Js.log("Complete in bubble"));

      ReasonReact.Update({...state, onScreen: v});
    },
  retainedProps: {
    step: step,
  },
  didUpdate: ({oldSelf, newSelf}) =>{
    if (oldSelf.retainedProps.step
        !== newSelf.retainedProps.step) {
      /* Animation */

      switch newSelf.retainedProps.step {
        | 0 => { newSelf.send(ResetValue);
        newSelf.send(TriggerAnim(false)); }
        | 1 => { newSelf.send(ResetValue);
      newSelf.send(TriggerAnim(true));
      newSelf.send(SetMessage("1/3")); }
      | 2 => { newSelf.send(ResetValue);
      newSelf.send(TriggerAnim(true));
      newSelf.send(SetMessage("1/4")); }
      };
      
    }},
  render: self =>
    /* self.send(TriggerAnim(shouldTriggerAnim)); */
    /*Parent view*/
    /* Js.log("inittial: " ++ string_of_bool(shouldTriggerAnim)); */
    <Animated.View
      style=Style.(
              style([
                borderColor(Colors.lemonTea),
                borderWidth(4.),
                position(Absolute),
                justifyContent(Center),
                right(Pt(circleDiameter)),
                top(Pt(0.)),
                alignItems(Center),
                borderRadius(circleDiameter /. 2.),
                width(Pt(circleDiameter /. 2.)),
                height(Pt(circleDiameter /. 2.)),
                backgroundColor(Colors.oyster),
                Transform.makeInterpolated(
                  ~scaleX=
                    Animated.Value.interpolate(
                      self.state.scale,
                      ~inputRange=[0., 1.],
                      ~outputRange=
                        `float(
                            self.state.onScreen ?
                            reverser.forward : reverser.backwards,
                        ),
                      (),
                    ),
                    ~scaleY=
                    Animated.Value.interpolate(
                      self.state.scale,
                      ~inputRange=[0., 1.],
                      ~outputRange=
                        `float(self.state.onScreen ?
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
                style([color(String("white")), fontSize(Float(14.))])
              )
        value=self.state.message
      />
    </Animated.View>,
};

/*
 #99badd => Karolina blue <3
  */