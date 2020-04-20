open ReactNative;

let styles =
  Style.(StyleSheet.create({"input": style(~borderWidth=1., ())}));

type item = {
  id: int,
  title: string,
  completed: bool,
};
module TodoItem = {
  [@react.component]
  let make = (~item, ~onToggle) => {
    <View>
      <Switch value={item.completed} onValueChange=onToggle />
      <Text> {React.string(item.title)} </Text>
    </View>;
  };
};

module Input = {
  type state = string;
  [@react.component]
  let make = (~onSubmit) => {
    let (text, setText) =
      React.useReducer((oldText, newText) => newText, "");
    <TextInput
      style=styles##input
      value=text
      defaultValue="Type here to add todo"
      onChangeText={text => setText(text)}
      onBlur={_ => setText("")}
      onSubmitEditing={_ => onSubmit(text, ())}
    />;
  };
};

type state = {items: list(item)};

type action =
  | AddItem(string)
  | ToggleItem(int);

let lastId = ref(0);
let newItem = text => {
  lastId := lastId^ + 1;
  {id: lastId^ + 1, title: text, completed: false};
};

[@react.component]
let make = () => {
  let ({items}, dispatch) =
    React.useReducer(
      (state, action) => {
        switch (action) {
        | AddItem(text) => {items: [newItem(text), ...state.items]}
        | ToggleItem(id) => {
            items:
              List.map(
                item =>
                  item.id === id
                    ? {...item, completed: !item.completed} : item,
                state.items,
              ),
          }
        }
      },
      {
        items: [{id: 0, title: "Write some things to do", completed: false}],
      },
    );
  <View>
    <Input onSubmit={(text, e) => dispatch(AddItem(text))} />
    <View>
      {List.map(
         item =>
           <TodoItem
             key={string_of_int(item.id)}
             onToggle={v => dispatch(ToggleItem(item.id))}
             item
           />,
         items,
       )
       |> Array.of_list
       |> React.array}
    </View>
    <View>
      <Text>
        {(
           List.filter(i => i.completed, items)->List.length->string_of_int
           ++ " items /"
         )
         ->React.string}
      </Text>
    </View>
  </View>;
};