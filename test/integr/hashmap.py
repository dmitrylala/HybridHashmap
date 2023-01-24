import subprocess
import pytest

SCRIPT_PATH = "./build/hashmap"


@pytest.mark.parametrize(
    "src, modify, remove, gt_result",
    [
        (
            {
                "hello": "world",
                "color": "orange",
                "language": "english",
            },
            {},
            {},
            {
                "hello": "world",
                "color": "orange",
                "language": "english",
            },
        ),
        (
            {
                "a": "b",
                "b": "c",
                "c": "d",
            },
            {
                "a": "c",
            },
            {
                "b",
            },
            {
                "a": "c",
                "c": "d",
            },
        ),
    ]
)
def test_hashmap(src, modify, remove, gt_result):
    with subprocess.Popen([SCRIPT_PATH],
                            stdin=subprocess.PIPE,
                            stdout=subprocess.PIPE,
                            stderr=subprocess.PIPE,
                            universal_newlines=True,
                            bufsize=0) as script:

        inputs = []
        for key, val in src.items():
            inputs.extend(["a", f"{key}\n{val}"])

        for key, val in modify.items():
            inputs.extend(["m", f"{key}\n{val}"])

        for key in remove:
            inputs.extend(["r", key])

        inputs.append("q\n")

        script.stdin.write("\n".join(inputs))
        script.stdin.close()

        result = {}
        lines = list(script.stdout)
        idx = 0
        for i, line in enumerate(lines):
            if "Your hashmap" in line:
                idx = i

        for line in lines[idx + 1:-1]:
            key, val = map(lambda x: x.strip(), line.split("###")[1].split(":"))
            result[key] = val

        assert result == gt_result
