open BsReactNative;

module Styles = {
  open Style;

  let container =
    style([
      flex(1.),
      backgroundColor(String("#EAE2D6"))
    ]);

  let text = style([color(String("#fff")), fontSize(Float(24.))]);
};

module Main = {

  let component = ReasonReact.statelessComponent("Main");

  let make = _children => {
    ...component,
    render: _self =>
      <View style=Styles.container>
        <IngridientTab />
      </View>
  };
};

let app = () => <Main />;
