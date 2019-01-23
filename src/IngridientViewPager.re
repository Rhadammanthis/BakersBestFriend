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
type ingridient =
  | Flour
  | Milk
  | Oil
  | BakingSoda
  | Cocoa
  | Cream
  | Sugar
  | PowderedSugar
  | Butter;

let convertIndex = (~index: int) => {
  let currentIngridient: ingridient =
    switch (index) {
    | 0 => Flour
    | 1 => Milk
    | 2 => Butter
    };
  currentIngridient;
};

type retainedProps = {
  sliderPosition: int,
  sliderOffset: float,
};

type state = {
  componentWidth: float,
  translateX: Animated.value(Animated.regular),
  foreground: Ingridients.metaData,
  background: Ingridients.metaData,
};
type action =
  | SetComponentWidth(float)
  | UpdateAnimatedValue(float)
  | UpdateImageAsset(int);

let component =
  ReasonReact.reducerComponentWithRetainedProps("IngridientViewPager");

let make =
    (~onLayout=?, ~sliderOffset: float, ~sliderPosition: int, _children) => {
  ...component,
  initialState: () => {
    componentWidth: 0.,
    translateX: Animated.Value.create(0.),
    foreground: {imageResource: Ingridients.find(0).image, coordinates: [0.,0.]},
    background: {imageResource: Ingridients.find(1).image, coordinates: [0.,0.]},
  },
  reducer: (action, state) =>
    switch (action) {
    | SetComponentWidth(f) =>
      ReasonReact.Update({...state, componentWidth: f, foreground: Ingridients.getForgroundData(sliderPosition, f),
        background: Ingridients.getBackgroundData(sliderPosition, f),})
    | UpdateAnimatedValue(anim) =>
      ReasonReact.Update({...state, translateX: Animated.Value.create(anim)})
    | UpdateImageAsset(i) =>

      ReasonReact.Update({
        ...state,
        foreground: Ingridients.getForgroundData(i, state.componentWidth),
        background: Ingridients.getBackgroundData(i, state.componentWidth),
      });
    },
  retainedProps: {
    sliderPosition,
    sliderOffset,
  },
  didUpdate: ({oldSelf, newSelf}) => {
    if (oldSelf.retainedProps.sliderOffset
        !== newSelf.retainedProps.sliderOffset) {
      newSelf.send(UpdateAnimatedValue(newSelf.retainedProps.sliderOffset));
    };

    if (oldSelf.retainedProps.sliderPosition
        !== newSelf.retainedProps.sliderPosition) {
      newSelf.send(UpdateImageAsset(newSelf.retainedProps.sliderPosition));
    };
  },
  render: self =>
    <View
      onLayout=(
        event => {
          let layoutData = RNEvent.NativeLayoutEvent.layout(event);
          self.send(SetComponentWidth(layoutData.width));
        }
      )
      style=Style.(style([flex(1.)]))>
      <View
        style=Style.(
                style([
                  flex(1.),
                  width(Pt(self.state.componentWidth *. 2.)),
                  flexDirection(Row),
                ])
              )>
        <View
          style=Style.(
                  style([
                    width(Pt(self.state.componentWidth)),
                    justifyContent(Center),
                    alignItems(Center),
                  ])
                )>
          <View
            style=Style.(
                    style([
                      backgroundColor(Colors.oyster),
                      borderColor(Colors.lemonTea),
                      borderWidth(4.),
                      borderRadius(self.state.componentWidth),
                      width(Pt(self.state.componentWidth *. 0.85)),
                      height(Pt(self.state.componentWidth *. 0.85)),
                      justifyContent(Center),
                      alignItems(Center),
                    ])
                  )>
            <Animated.Image
              style=Style.(
                      style([
                        height(Pt(180.)),
                        width(Pt(180.)),
                        Transform.makeInterpolated(
                          ~translateX=
                            Animated.Value.interpolate(
                              self.state.translateX,
                              ~inputRange=[0, 1] |> List.map(float),
                              ~outputRange=
                                `float(self.state.foreground.coordinates),
                              (),
                            ),
                          (),
                        ),
                      ])
                    )
              source=(`Required(self.state.foreground.imageResource))
              resizeMode=`stretch
            />
          </View>
        </View>
        <View
          style=Style.(
                  style([
                    width(Pt(self.state.componentWidth)),
                    justifyContent(Center),
                    alignItems(Center),
                  ])
                )>
          <Animated.Image
            style=Style.(
                    style([
                      height(Pt(180.)),
                      width(Pt(180.)),
                      Transform.makeInterpolated(
                        ~translateX=
                          Animated.Value.interpolate(
                            self.state.translateX,
                            ~inputRange=[0, 1] |> List.map(float),
                            ~outputRange=
                              `float(self.state.background.coordinates),
                            (),
                          ),
                        (),
                      ),
                    ])
                  )
            source=(`Required(self.state.background.imageResource))
            resizeMode=`stretch
          />
        </View>
      </View>
    </View>,
};

/*
 #99badd => Karolina blue <3
  */