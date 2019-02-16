open BsReactNative;

type state = {
  animation: Animated.value(Animated.regular),
  played: bool,
};
type action =
  | StartAnim(bool)
  | Spring;

let component = ReasonReact.reducerComponent("BubbleButton");

let renderIndicator =
    (~text, ~image: option(BsReactNative.Packager.required), ~diameter) =>
  switch (image) {
  | None =>
    <Text
      style=Style.(
              style([
                color(String("white")),
                fontSize(Float(17.)),
                fontWeight(`Bold),
              ])
            )
      value=text
    />
  | Some(imgResource) =>
    <Image
      style=Style.(style([height(Pt(diameter /. 2.)), width(Pt(diameter /. 2.))]))
      source=(`Required(imgResource))
      resizeMode=`stretch
    />
  };

let make =
    (
      ~style,
      ~text="",
      ~onPress,
      ~diameter,
      ~image: option(BsReactNative.Packager.required)=None,
      _children,
    ) => {
  ...component,
  initialState: () => {animation: Animated.Value.create(0.), played: false},
  reducer: (action, state) =>
    switch (action) {
    | Spring =>
      let animation =
        Animated.spring(
          ~value=state.animation,
          ~toValue=`raw(0.0),
          ~friction=3.5,
          ~tension=35.,
          (),
        );

      Animated.start(animation, ());

      ReasonReact.Update({...state, played: ! state.played});
    | StartAnim(v) =>
      let animation =
        Animated.timing(
          ~value=state.animation,
          ~toValue=`raw(v ? 1.0 : 0.),
          ~duration=100.0,
          (),
        );

      Animated.start(animation, ());

      ReasonReact.Update({...state, played: ! state.played});
    },
  render: self => {
    /* self.send(TriggerAnim(shouldTriggerAnim)); */
    /*Parent view*/
    /* Js.log("inittial: " ++ string_of_bool(shouldTriggerAnim)); */

    let newStyle =
      BsReactNative.Style.combine(
        style,
        Style.(
          style([
            justifyContent(Center),
            alignItems(Center),
            borderRadius(diameter),
            width(Pt(diameter)),
            height(Pt(diameter)),
            Transform.makeInterpolated(
              ~scaleX=
                Animated.Value.interpolate(
                  self.state.animation,
                  ~inputRange=[0., 1.],
                  ~outputRange=`float([1., 1.25]),
                  (),
                ),
              ~scaleY=
                Animated.Value.interpolate(
                  self.state.animation,
                  ~inputRange=[0., 1.],
                  ~outputRange=`float([1., 1.25]),
                  (),
                ),
              (),
            ),
          ])
        ),
      );

    <Animated.View style=newStyle>
      <TouchableWithoutFeedback
        style=newStyle
        onPressIn=(
          () => {
            Js.log("innnn");
            self.send(StartAnim(true));
          }
        )
        onPressOut=(
          () => {
            Js.log("out");
            self.send(StartAnim(false));
          }
        )
        onPress=(
          () => {
            Js.log("pressed");
            self.send(Spring);
            onPress();
          }
        )>
        <View
          style=Style.(
                  style([
                    width(Pt(diameter)),
                    height(Pt(diameter)),
                    borderRadius(diameter),
                    justifyContent(Center),
                    alignItems(Center),
                  ])
                )>
          (renderIndicator(~text, ~image, ~diameter))
        </View>
      </TouchableWithoutFeedback>
    </Animated.View>;
  },
};

/*
 #99badd => Karolina blue <3
  */