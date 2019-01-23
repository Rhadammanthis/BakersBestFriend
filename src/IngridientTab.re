open BsReactNative;

type imageResource = {
  first: BsReactNative.Packager.required,
  second: BsReactNative.Packager.required,
};

type ingridient =
  | Flour
  | Honey
  | Sugar;

type coordinates = {
  right: float,
  left: float,
};

type pair = {
  first: coordinates,
  second: coordinates,
};

let convertIndex = (~index: int) => {
  let currentIngridient: ingridient =
    switch (index) {
    | 0 => Flour
    | 1 => Honey
    | 2 => Sugar
    };
  currentIngridient;
};

module Styles = {
  open Style;

  let container =
    style([
      flex(9.),
      flexDirection(Row),
      justifyContent(FlexStart),
      padding(Pt(10.)),
      backgroundColor(String("#EAE2D6")),
    ]);

  let rightPanel =
    style([
      flex(1.),
      flexDirection(Column),
      backgroundColor(String("#EAE2D6")),
    ]);

  let bottomTab =
    style([
      backgroundColor(String("#867666")),
      alignItems(Center),
      justifyContent(Center),
    ]);

  let text = style([color(String("#fff")), fontSize(Float(24.))]);
};

type state = {
  algo: int,
  initial: float,
  converted: float,
  sliderUnit: int,
  sliderUnitMessage: string,
  translateX: float,
  imageContainerWidth: float,
  ingridientAssets: imageResource,
  currentIngridient: int,
  sliderHasFocus: bool,
};
type action =
  | ChangeAlgo(int)
  | UpdateConvertedAmount(float)
  | UpdateInitialAmount(float)
  | ChangeSliderUnit(int)
  | UpdateAnimatedValue(float)
  | UpdateImageContainerWidth(float)
  | SetCurrentIngridient(int)
  | UpdateSliderFocus(bool);

let component = ReasonReact.reducerComponent("Main");

let circleDiameter = 100.;
let textSize = 18.;

let animatedValue = Animated.Value.create(1.0);

/* let listener = (~step: int) => {
     Js.log("On step number: " ++ string_of_int(step));
   }; */

let make = _children => {
  ...component,
  initialState: () => {
    algo: 0,
    initial: 0.,
    converted: 0.,
    sliderUnit: 0,
    sliderUnitMessage: "Half a cup (1/2)",
    translateX: 0.,
    imageContainerWidth: 0.,
    ingridientAssets: {
      first: Packager.require("../../../assets/flour.png"),
      second: Packager.require("../../../assets/butter.png"),
    },
    currentIngridient: 0,
    sliderHasFocus: false,
  },
  reducer: (action, state) =>
    switch (action) {
    | ChangeAlgo(v) => ReasonReact.Update({...state, algo: v})
    | UpdateConvertedAmount(f) => ReasonReact.Update({...state, converted: f})
    | UpdateInitialAmount(f) => ReasonReact.Update({...state, initial: f})
    | SetCurrentIngridient(i) =>
      ReasonReact.Update({...state, currentIngridient: i})

    | UpdateImageContainerWidth(w) =>
      ReasonReact.Update({...state, imageContainerWidth: w})
    | UpdateAnimatedValue(anim) =>
      ReasonReact.Update({...state, translateX: anim})
    | UpdateSliderFocus(b) =>
      ReasonReact.Update({...state, sliderHasFocus: b})
    | ChangeSliderUnit(u) =>
      let message =
        switch (u) {
        | 0 => "Half cup (1/2)"
        | 1 => "Third cup (3/2)"
        | 2 => "Quarter cup (4/2)"
        };

      ReasonReact.Update({
        ...state,
        sliderUnit: u,
        sliderUnitMessage: message,
      });
    },
  render: self =>
    <View style=Style.(style([flex(1.), paddingTop(Pt(20.))]))>
      <View style=Styles.container>
        <VerticalSlider
          onStepMet=(
            (step: int) => {
              self.send(UpdateInitialAmount(float_of_int(step) *. 0.5));
              self.send(UpdateConvertedAmount(float_of_int(step) *. 60.));
            }
          )
          onHorizontalStep=(
            (horizontalStep: int) => {
              self.send(ChangeSliderUnit(horizontalStep));
              Js.log("Horizontal step: " ++ string_of_int(horizontalStep));
            }
          )
          hasFocus=(
            (hasFocus: bool) => self.send(UpdateSliderFocus(hasFocus))
          )
        />
        <View style=Styles.rightPanel>
          <View
            style=Style.(
                    style([
                      flex(1.),
                      justifyContent(FlexStart),
                      alignItems(FlexEnd),
                      paddingTop(Pt(30.)),
                    ])
                  )>
            <View
              style=Style.(
                      style([
                        borderColor(String("#E1B80D")),
                        borderWidth(4.),
                        position(Absolute),
                        justifyContent(Center),
                        alignItems(Center),
                        borderRadius(circleDiameter),
                        width(Pt(circleDiameter)),
                        height(Pt(circleDiameter)),
                        backgroundColor(String("#D5C3AA")),
                      ])
                    )>
              <View style=Style.(style([]))>
                <Text
                  style=Style.(
                          style([
                            color(String("white")),
                            fontSize(Float(textSize)),
                            marginTop(Pt(-25.)),
                          ])
                        )
                  value="Cups"
                />
              </View>
              <View>
                <Text
                  style=Style.(
                          style([
                            color(String("white")),
                            fontSize(Float(textSize)),
                            marginLeft(Pt(circleDiameter /. 2.5)),
                          ])
                        )
                  value=(string_of_float(self.state.initial))
                />
              </View>
            </View>
            <View
              style=Style.(
                      style([
                        right(Pt(circleDiameter /. 3.)),
                        top(Pt(circleDiameter /. 2.5)),
                        borderColor(String("#E1B80D")),
                        borderWidth(8.),
                        position(Absolute),
                        justifyContent(Center),
                        alignItems(Center),
                        borderRadius(circleDiameter),
                        width(Pt(circleDiameter)),
                        height(Pt(circleDiameter)),
                        backgroundColor(String("#D5C3AA")),
                      ])
                    )>
              <Text
                style=Style.(
                        style([
                          color(String("white")),
                          fontSize(Float(textSize)),
                        ])
                      )
                value=(string_of_float(self.state.converted) ++ " gr.")
              />
            </View>
            <FractionBubble
            step=self.state.sliderUnit
            /* shouldTriggerAnim=self.state.sliderHasFocus */
            />

          </View>
          <View style=Style.(style([flex(2.), marginBottom(Pt(50.))]))>
            <IngridientViewPager
              sliderOffset=self.state.translateX
              sliderPosition=self.state.currentIngridient
            />
          </View>
        </View>
      </View>
      <ViewPagerAndroid
        peekEnabled=true
        initialPage=0
        onPageScroll=(
          event => {
            self.send(
              UpdateAnimatedValue(float_of_int(event##nativeEvent##offset)),
            );
            if (event##nativeEvent##position !== self.state.currentIngridient) {
              self.send(SetCurrentIngridient(event##nativeEvent##position));
            };
          }
        )
        style=Style.(style([flex(1.), height(Pt(150.))]))>
        <View style=Styles.bottomTab key="0">
          <Text
            style=Style.(
                    style([fontSize(Float(18.)), color(String("white"))])
                  )
            value="Milk"
          />
        </View>
        <View style=Styles.bottomTab key="1">
          <Text
            style=Style.(
                    style([fontSize(Float(18.)), color(String("white"))])
                  )
            value="Flour"
          />
        </View>
        <View style=Styles.bottomTab key="2">
          <Text
            style=Style.(
                    style([fontSize(Float(18.)), color(String("white"))])
                  )
            value="Butter"
          />
        </View>
        <View style=Styles.bottomTab key="3">
          <Text
            style=Style.(
                    style([fontSize(Float(18.)), color(String("white"))])
                  )
            value="Baking Soda"
          />
        </View>
        <View style=Styles.bottomTab key="4">
          <Text
            style=Style.(
                    style([fontSize(Float(18.)), color(String("white"))])
                  )
            value="Cocoa"
          />
        </View>
        <View style=Styles.bottomTab key="5">
          <Text
            style=Style.(
                    style([fontSize(Float(18.)), color(String("white"))])
                  )
            value="Cream"
          />
        </View>
        <View style=Styles.bottomTab key="6">
          <Text
            style=Style.(
                    style([fontSize(Float(18.)), color(String("white"))])
                  )
            value="Oil"
          />
        </View>
        <View style=Styles.bottomTab key="7">
          <Text
            style=Style.(
                    style([fontSize(Float(18.)), color(String("white"))])
                  )
            value="Powdered Sugar"
          />
        </View>
        <View style=Styles.bottomTab key="8">
          <Text
            style=Style.(
                    style([fontSize(Float(18.)), color(String("white"))])
                  )
            value="Sugar"
          />
        </View>
      </ViewPagerAndroid>
      <DropDownBar
        shouldTriggerAnim=self.state.sliderHasFocus
        step=self.state.sliderUnit
      />
    </View>,
};