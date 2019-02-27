open BsReactNative;

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
  sliderVal: int,
  translateX: float,
  currentIngridient: int,
  measurementUnit: Ingridients.measurementUnits,
  extraCups: int,
};
type action =
  | UpdateSliderValue(int)
  | UpdateAnimatedValue(float)
  | SetCurrentIngridient(int)
  | ChangeMesurementUnit(Ingridients.measurementUnits)
  | AddExtraCup(int);

let component = ReasonReact.reducerComponent("Main");

let circleDiameter = 110.;
let textSize = 17.;

let animatedValue = Animated.Value.create(1.0);

let unitFactor = (unit: Ingridients.measurementUnits) => {
  let factor =
    switch (unit) {
    | Imperial => 0.035274
    | Metric => 1.
    };
  factor;
};

let generateIngridientsStubs = (~lenght: int) => {
  Js.log(lenght);
  let ingridients: array(int) = Array.make(lenght, 0);
  for (i in 0 to lenght - 1) {
    ingridients[i] = i;
  };

  let ingridientComponents =
    List.map(
      ingridientItem => 
        <View key=(string_of_int(ingridientItem)) style=Styles.bottomTab>
          <Text
            style=Style.(
                    style([fontSize(Float(18.)), color(Colors.white)])
                  )
            value=Ingridients.find(ingridientItem).name
          />
        </View>
      ,
      Array.to_list(ingridients),
    );

    ingridientComponents;
};

let make = _children => {
  ...component,
  initialState: () => {
    sliderVal: 0,
    translateX: 0.,
    currentIngridient: 0,
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
    | UpdateSliderValue(f) => ReasonReact.Update({...state, sliderVal: f})
    | SetCurrentIngridient(i) =>
      ReasonReact.Update({...state, currentIngridient: i})
    | UpdateAnimatedValue(anim) =>
      ReasonReact.Update({...state, translateX: anim})

    | ChangeMesurementUnit(u) =>
      let newUnit =
        switch (u) {
        | Imperial => Ingridients.Metric
        | Metric => Ingridients.Imperial
        };
      ReasonReact.Update({...state, measurementUnit: newUnit});
    },
  render: self =>
    <View style=Style.(style([flex(1.), paddingTop(Pt(10.))]))>
      <View style=Styles.container>
        <VerticalSlider
          onStepMet=((step: int) => self.send(UpdateSliderValue(step)))
          onHorizontalStep=(
            (horizontalStep: int) =>
              Js.log("Horizontal step: " ++ string_of_int(horizontalStep))
          )
          hasFocus=((hasFocus: bool) => Js.log("Has focus!"))
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
                            color(Colors.white),
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
                            color(Colors.white),
                            fontSize(Float(textSize +. 10.)),
                            paddingLeft(Pt(circleDiameter /. 2.5)),
                          ])
                        )
                  value=(
                    Parse.message(self.state.sliderVal, self.state.extraCups)
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
                          color(Colors.white),
                          fontSize(Float(textSize +. 10.)),
                          textAlign(Center),
                        ])
                      )
                value=(
                  Parse.convertedMagnitude(
                    ~step=self.state.sliderVal,
                    ~extraCups=self.state.extraCups,
                    ~ingridientUuid=self.state.currentIngridient,
                    ~unit=self.state.measurementUnit,
                  )
                )
              />
              <Text
                style=Style.(
                        style([
                          color(Colors.white),
                          fontSize(Float(textSize)),
                          marginTop(Pt(8.)),
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
                | Metric =>
                  Some(
                    BsReactNative.Packager.require(
                      "../../../assets/metric.png",
                    ),
                  )
                | Imperial =>
                  Some(
                    BsReactNative.Packager.require(
                      "../../../assets/imperial.png",
                    ),
                  )
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
        (ReasonReact.array(
            Array.of_list(
              generateIngridientsStubs(
                ~lenght=Ingridients.lenght - 1
              ),
            ),
          ))
      </ViewPagerAndroid>
    </View>,
};