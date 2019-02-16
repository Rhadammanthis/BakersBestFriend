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
  right: int,
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
      backgroundColor(Colors.linen),
    ]);

  let rightPanel =
    style([
      flex(1.),
      flexDirection(Column),
      backgroundColor(Colors.linen),
    ]);

  let bottomTab =
    style([
      backgroundColor(Colors.pewter),
      alignItems(Center),
      justifyContent(Center),
    ]);

  let text = style([color(String("#fff")), fontSize(Float(24.))]);
};

type state = {
  algo: int,
  initial: int,
  converted: int,
  sliderUnit: int,
  sliderUnitMessage: string,
  translateX: float,
  imageContainerWidth: float,
  ingridientAssets: imageResource,
  currentIngridient: int,
  sliderHasFocus: bool,
  measurementUnit: Ingridients.measurementUnits,
  extraCups: int,
};
type action =
  | ChangeAlgo(int)
  | UpdateConvertedAmount(int)
  | UpdateInitialAmount(int)
  | ChangeSliderUnit(int)
  | UpdateAnimatedValue(float)
  | UpdateImageContainerWidth(float)
  | SetCurrentIngridient(int)
  | UpdateSliderFocus(bool)
  | ChangeMesurementUnit(Ingridients.measurementUnits)
  | AddExtraCup(int);

let component = ReasonReact.reducerComponent("Main");

let circleDiameter = 110.;
let textSize = 17.;

let animatedValue = Animated.Value.create(1.0);

let unitFactor = (unit: Ingridients.measurementUnits) => {
  let factor =
    switch (unit) {
    | Imperial => 0.035
    | Metric => 1.
    };
  factor;
};

/*0.035274
  }; */

let make = _children => {
  ...component,
  initialState: () => {
    algo: 0,
    initial: 0,
    converted: 0,
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
    measurementUnit: Metric,
    extraCups: 0,
  },
  reducer: (action, state) =>
    switch (action) {
    | AddExtraCup(i) =>
      ReasonReact.Update({
        ...state,
        extraCups: state.extraCups + i > 0 ? state.extraCups + i : 0,
      })
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
    | ChangeMesurementUnit(u) =>
      let newUnit =
        switch (u) {
        | Imperial => Ingridients.Metric
        | Metric => Ingridients.Imperial
        };
      ReasonReact.Update({...state, measurementUnit: newUnit});
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
    <View style=Style.(style([flex(1.), paddingTop(Pt(10.))]))>
      <View style=Styles.container>
        <VerticalSlider
          onStepMet=(
            (step: int) => {
              self.send(UpdateInitialAmount(step));
              self.send(UpdateConvertedAmount(step));
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
                      marginRight(Pt(25.)),
                    ])
                  )>
            <View
              style=Style.(
                      style([
                        borderColor(Colors.lemonTea),
                        borderWidth(4.),
                        position(Absolute),
                        justifyContent(Center),
                        alignItems(Center),
                        borderRadius(circleDiameter),
                        width(Pt(circleDiameter)),
                        height(Pt(circleDiameter)),
                        backgroundColor(Colors.oyster),
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
              <View
                style=Style.(
                        style([
                          flexDirection(Row),
                          justifyContent(FlexEnd),
                          alignItems(Center),
                        ])
                      )>
                <Text
                  style=Style.(
                          style([
                            color(String("white")),
                            fontSize(Float(textSize +. 10.)),
                            paddingLeft(Pt(circleDiameter /. 2.5)),
                          ])
                        )
                  value=(
                    Parse.message(self.state.initial, self.state.extraCups)
                  )
                />
              </View>
            </View>
            <View
              style=Style.(
                      style([
                        right(Pt(circleDiameter /. 2.5)),
                        top(Pt(circleDiameter /. 2.5)),
                        borderColor(Colors.lemonTea),
                        borderWidth(6.),
                        position(Absolute),
                        justifyContent(Center),
                        alignItems(Center),
                        borderRadius(circleDiameter),
                        width(Pt(circleDiameter)),
                        height(Pt(circleDiameter)),
                        backgroundColor(Colors.oyster),
                        flex(1.),
                        flexDirection(Row),
                      ])
                    )>
              <Text
                adjustsFontSizeToFit=true
                minimumFontScale=0.01
                style=Style.(
                        style([
                          color(String("white")),
                          fontSize(Float(textSize +. 10.)),
                          textAlign(Center),
                        ])
                      )
                value=(
                  Parse.convertedMagnitude(
                    ~step=self.state.initial,
                    ~extraCups=self.state.extraCups,
                    ~ingridientUuid=self.state.currentIngridient,
                    ~unit=self.state.measurementUnit,
                  )
                )
              />
              <Text 
              style=Style.(
                        style([
                          color(String("white")),
                          fontSize(Float(textSize)),
                          marginTop(Pt(8.))
                        ])
                      )
              value=(
                switch (self.state.measurementUnit) {
                | Metric =>
                  switch (
                    Ingridients.find(self.state.currentIngridient).unit
                  ) {
                  | Ingridients.Liquid => " ml."
                  | Ingridients.Solid => " gr."
                  }
                | Imperial =>
                  switch (
                    Ingridients.find(self.state.currentIngridient).unit
                  ) {
                  | Ingridients.Liquid => " floz."
                  | Ingridients.Solid => " oz."
                  }
                }
              )
              />
            </View>
            <BubbleButton
              onPress=(
                () =>
                  self.send(ChangeMesurementUnit(self.state.measurementUnit))
              )
              diameter=(circleDiameter /. 2.)
              image=(
                switch (self.state.measurementUnit) {
                | Metric => Some(BsReactNative.Packager.require("../../../assets/milk.png"))
                | Imperial => Some(BsReactNative.Packager.require("../../../assets/milk.png"))
                }
              )
              style=Style.(
                      style([
                        position(Absolute),
                        right(Pt(circleDiameter)),
                        top(Pt(-5.)),
                        backgroundColor(Colors.light_pewter),
                      ])
                    )
            />
            <View
              style=Style.(
                      style([
                        position(Absolute),
                        right(Pt(-. circleDiameter /. 4.)),
                        top(Pt(circleDiameter -. circleDiameter /. 4.0)),
                        marginRight(Pt(5.)),
                        flexDirection(Column),
                        flex(1.),
                        justifyContent(SpaceAround),
                      ])
                    )>
              <BubbleButton
                onPress=(() => self.send(AddExtraCup(1)))
                diameter=(circleDiameter /. 3.2)
                text="+"
                style=Style.(style([backgroundColor(Colors.light_pewter)]))
              />
              <BubbleButton
                onPress=(() => self.send(AddExtraCup(-1)))
                diameter=(circleDiameter /. 3.2)
                text="-"
                style=Style.(
                        style([
                          marginTop(Pt(-8.)),
                          backgroundColor(Colors.light_pewter),
                          marginLeft(Pt(-. circleDiameter /. 3.8)),
                        ])
                      )
              />
            </View>
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
                    style([fontSize(Float(20.)), color(String("white"))])
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
                    style([fontSize(Float(20.)), color(String("white"))])
                  )
            value="Sugar"
          />
        </View>
      </ViewPagerAndroid>
    </View>,
};