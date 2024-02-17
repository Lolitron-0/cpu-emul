#include "Executor.hpp"
#include <boost/program_options.hpp>

bool ValidateProgramOptions(const boost::program_options::variables_map& vm)
{
    return vm.contains("source") != vm.count("binary") &&
           (!vm.contains("export-binary") ||
            vm.contains("source")); // implication export-binary => source
}

int main(int argc, char** argv)
{
	cpuemul::Executor::GetInstance().LoadFromSource("");
	return 0;
    namespace opts = boost::program_options;

    std::string sourcePath, binaryPath;

    opts::options_description optsDescription{ "Allowed opitons" };
    std::string oneRequiredNote{
        "(note: only one from --binary and --source should be specified)"
    };

    // clang-format off
    optsDescription.add_options()
		("help,h", "display help")
		("source,s", 
		 opts::value<std::string>(&sourcePath),
		 ("specify source code file "+oneRequiredNote).c_str())
		("binary,b",
		 opts::value<std::string>(&binaryPath),
        ("specify binary file " + oneRequiredNote).c_str())
		("export-binary,e",
        "if set, yields binary from source code (--source required)");
    // clang-format on

    opts::variables_map vm;
    opts::store(opts::parse_command_line(argc, argv, optsDescription), vm);

    if (vm.count("help"))
    {
        std::cout << optsDescription << std::endl;
        return 0;
    }

    opts::notify(vm);

    if (!ValidateProgramOptions(vm))
    {
        std::cout << optsDescription << std::endl;
        return 0;
    }

    if (vm.contains("binary"))
    {
        cpuemul::Executor::GetInstance().LoadFromBinary(binaryPath);
    }
    else
    {
        cpuemul::Executor::GetInstance().LoadFromSource(sourcePath);
    }

    cpuemul::Executor::GetInstance().Run();
    return 0;
}
