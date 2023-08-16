#!/usr/bin/env python3
# -*- coding: utf-8 -*-
#
# This file is part of Nike.
#
# Nike is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# Nike is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with Nike.  If not, see <https://www.gnu.org/licenses/>.
#
import re
import sys
from copy import copy
from pathlib import Path

from graphviz import Graph, Digraph


def main():
    file = Path(sys.argv[1]).absolute()

    g = Digraph()
    g.node("fake")

    visited = set()
    state_stack = []
    var_stack = []
    move_stack = []

    for row in file.read_text().splitlines(keepends=False):

        m = re.search(" visit (system|env) node ([0-9]+)", row)
        if m is not None:
            statetype = m.group(1)
            state = m.group(2)
            g.node(
                state, label=state, shape=("circle" if statetype == "system" else "box")
            )
            state_stack.append((statetype, state))

            previousstatetype, previousstate = (
                state_stack[-2] if len(state_stack) > 1 else ("", "fake")
            )
            last_move_type, last_move = (
                move_stack[-1] if len(move_stack) > 0 else ("", [])
            )
            label = (
                "\n".join([f"{x}={v}" for _, x, v in last_move])
                if len(last_move) > 0
                else "true"
            )
            g.edge(previousstate, state, label=label)
            continue

        m = re.search(
            "branch on (system|env) variable ([a-zA-Z0-9_]+) \(([01])\)$", row
        )
        if m is not None:
            vartype = m.group(1)
            varname = m.group(2)
            varval = bool(m.group(3) == "1")
            var_stack.append((vartype, varname, varval))
            continue

        m = re.search(
            "no (uncontrollable|controllable) variables -> find (next system|env) move",
            row,
        )
        if m is not None:
            movetype = m.group(1)
            move_stack.append((movetype, copy(var_stack)))
            var_stack = []
            continue

        m = re.search("([0-9]+) accepting", row)
        if m is not None:
            statename = m.group(1)
            make_state_accepting(statename, g)
            state_stack.pop()
            movetype, move_vars = move_stack.pop()
            var_stack = move_vars
            continue

        m = re.search(
            "branch on (system|env) variable ([a-zA-Z0-9_-]+) \(([01])\) (SUCCESS|FAILURE)",
            row,
        )
        if m is not None:
            vartype = m.group(1)
            varname = m.group(2)
            varval = m.group(3)

            previousvartype, previousvarname, previousvarval = var_stack.pop()
            assert vartype == previousvartype
            assert varname == previousvarname
            continue

        m = re.search("Loop detected for node ([0-9]+), tagging the node", row)
        if m is not None:
            expectedstate = m.group(1)
            _, actualstate = state_stack.pop()
            assert expectedstate == actualstate
            movetype, move_vars = move_stack.pop()
            var_stack = move_vars
            continue

        m = re.search("agent state ([0-9]+) already discovered, (success|failure)", row)
        if m is not None:
            expectedstate = m.group(1)
            success_or_failure = m.group(2)
            _, actualstate = state_stack.pop()
            assert expectedstate == actualstate
            movetype, move_vars = move_stack.pop()
            var_stack = move_vars
            continue

        m = re.search(
            "One-step (realizability|unrealizability) success for node ([0-9]+)", row
        )
        if m is not None:
            checkresult = m.group(1)
            expectedstate = m.group(2)
            _, actualstate = state_stack.pop()
            assert expectedstate == actualstate
            movetype, move_vars = move_stack.pop()
            var_stack = move_vars

            if checkresult == "realizability":
                make_state_accepting(expectedstate, g, comment="one-step")
            if checkresult == "unrealizability":
                make_state_rejecting(expectedstate, g, comment="one-step")

            continue

        m = re.search("env can force agent failure from state ([0-9]+)", row)
        if m is not None:
            expected_state = m.group(1)
            currentstatetype, current_state = state_stack.pop()
            assert expected_state == current_state
            assert currentstatetype == "env"
            movetype, move_vars = move_stack.pop()
            var_stack = move_vars
            make_state_rejecting(current_state, g)

        m = re.search("all env moves lead to success from state ([0-9]+)", row)
        if m is not None:
            expected_state = m.group(1)
            currentstatetype, current_state = state_stack.pop()
            assert expected_state == current_state
            assert currentstatetype == "env"
            assert len(var_stack) == 0
            movetype, move_vars = move_stack.pop()
            var_stack = move_vars
            make_state_accepting(current_state, g)

        m = re.search("found winning strategy at state ([0-9]+)", row)
        if m is not None:
            expected_state = m.group(1)
            currentstatetype, current_state = state_stack.pop()
            assert expected_state == current_state
            assert currentstatetype == "system"
            assert len(var_stack) == 0

            if len(state_stack) > 0:
                # not in the initial state
                movetype, move_vars = move_stack.pop()
                var_stack = move_vars
                make_state_accepting(current_state, g)

    assert len(var_stack) == 0
    assert len(move_stack) == 0
    g.render(file.name)


def add_attributes_to_state(statename, g, color="", comment=""):
    index = None
    for index, b in enumerate(g.body):
        if b.startswith(f"\t{statename} [label={statename}"):
            break
    assert index != (len(g.body) - 1)
    if color != "":
        g.body[index] = (
            g.body[index]
            .replace(
                "shape=circle", f"shape=doublecircle,fillcolor={color},style=filled"
            )
            .replace("shape=box", f"shape=doubleoctagon,fillcolor={color},style=filled")
        )
    if comment != "":
        g.body[index] = g.body[index].replace(
            f"label={statename} ", f'label="{statename},\n{comment}" '
        )


def make_state_accepting(statename, g, comment=""):
    add_attributes_to_state(statename, g, color="green", comment=comment)


def make_state_rejecting(statename, g, comment=""):
    add_attributes_to_state(statename, g, color="red", comment=comment)


if __name__ == "__main__":
    main()
